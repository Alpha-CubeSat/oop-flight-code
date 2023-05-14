import platform
import os
import time
import collections
import threading

if platform.system() == 'Windows':
    import windows_curses as curses
else:
    import curses

class Logger:
  def __init__(self, stdscr):
    self.stdscr = stdscr
    curses.cbreak()
    self.stdscr.keypad(True)
    self.stdscr.nodelay(True)

    curses.init_pair(1, curses.COLOR_CYAN, curses.COLOR_BLACK)
    self.stdscr.attron(curses.color_pair(1))

    self.print_queue = []
    self.command_history = []
    self.command_queue = collections.deque()

    self.user_input = b''
    self.command_length = 0
    self.command_pointer = -1
    self.last_height, self.last_width = stdscr.getmaxyx()

    self.prompt_thread = threading.Thread(target=self.prompt_loop)
    self.prompt_thread.daemon = True
    self.prompt_thread.start()

  def queue_output_for_printing(self, output):
    self.print_queue.append(output)

  def prompt_loop(self):
    while True:
      self.run_log_cycle()
  
  def run_log_cycle(self):

    # self.stdscr.clear()

    height, width = self.stdscr.getmaxyx()

    if height != self.last_height or width != self.last_width:
        curses.resizeterm(height, width)
        last_height = height
        last_width = width

    self.stdscr.addstr(height-1, 0, '>> ')
    
    curses.curs_set(0)
    

    # Print command history
    for i in range(min(len(self.print_queue),height-3)):
        self.stdscr.addstr(height-3-i, 0, ' ' * (width-1))
        self.stdscr.addstr(height-3-i, 0, self.print_queue[len(self.print_queue) - i - 1])

    # Move cursor to user input position
    
    # Get user input
    try:
        key = self.stdscr.get_wch()
        if key == '\n':
            # Add user input to command history
            self.print_queue.append(self.user_input)
            self.command_history.append(self.user_input)
            self.command_queue.append(self.user_input)
            # Clear user input
            self.user_input = b''
            self.command_length = 0
            self.command_pointer = -1
        elif key == curses.KEY_RESIZE:
          height, width = self.stdscr.getmaxyx()
          curses.resizeterm(height, width)
          last_height = height
          last_width = width
        elif key == curses.KEY_UP:
          if len(self.command_history)-1>self.command_pointer:
            self.command_pointer = self.command_pointer + 1
            self.user_input = self.command_history[-self.command_pointer-1]
            self.command_length = len(self.user_input)
        elif key == curses.KEY_DOWN:
          if 1<=self.command_pointer:
            self.command_pointer = self.command_pointer - 1
            self.user_input = self.command_history[-self.command_pointer-1]
            self.command_length = len(self.user_input)
          elif 0 == self.command_pointer:
            self.command_pointer = self.command_pointer -1
            self.user_input = b''
            self.command_length = 0
        elif key == curses.KEY_BACKSPACE or ord(key) == 127:
            # Handle backspace
            self.command_pointer = -1
            if self.command_length>0:
              self.user_input = self.user_input[:-1]
              self.command_length = self.command_length -1
        else:
            self.command_length = self.command_length + 1
            # Add key to user input
            self.command_pointer = -1
            self.user_input += key.encode()
    except curses.error:
        # Ignore keypad errors
        pass

    self.stdscr.addstr(height-2, 0, '_'*(width-1))
    # Print user input
    # curses.echo()
    self.stdscr.addstr(height-1, 3, ' '*(width-4))
    self.stdscr.addstr(height-1, 3, self.user_input.decode())
    self.stdscr.addstr(height-1, self.command_length+3,'█')
    # stdscr.addstr(height-1, command_length+3,str(command_pointer))

    # print_queue.append("Test")
    # stdscr.move(height-1, command_length + 3)


    # Refresh screen
    self.stdscr.refresh()

  def fetch_queued_commands(self):
    queue_to_return = self.command_queue.copy()
    self.command_queue.clear()
    return queue_to_return