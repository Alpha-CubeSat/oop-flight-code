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
    #Initialize curses screen
    self.stdscr = stdscr
    curses.cbreak()
    self.stdscr.keypad(True)
    self.stdscr.nodelay(True)

    # Set Color for console
    curses.init_pair(1, curses.COLOR_CYAN, curses.COLOR_BLACK)
    self.stdscr.attron(curses.color_pair(1))

    #Initialize queues, iterators
    self.print_queue = []
    self.command_history = []
    self.command_queue = collections.deque()

    self.user_input = b''
    self.command_length = 0
    self.command_pointer = -1
    self.last_height, self.last_width = stdscr.getmaxyx()

    #Start the debug console on its own thread to speed up input responsiveness
    self.prompt_thread = threading.Thread(target=self.prompt_loop)
    self.prompt_thread.daemon = True
    self.prompt_thread.start()

  # Takes the output and adds it to a queue for the debug_console to print
  def queue_output_for_printing(self, output):
    self.print_queue.append(output)

  # Starts debug console on its own
  def prompt_loop(self):
    while True:
      self.run_log_cycle()
  
  # Runs a single cycle of the debug console
  def run_log_cycle(self):

    # self.stdscr.clear()

    

    # resize the console based on terminal width and height
    height, width = self.stdscr.getmaxyx()
    if height != self.last_height or width != self.last_width:
        curses.resizeterm(height, width)
        last_height = height
        last_width = width
    
    # Adds Prompt
    self.stdscr.addstr(height-1, 0, '>> ')
    curses.curs_set(0)
    

    # Print command history
    for i in range(min(len(self.print_queue),height-3)):
        self.stdscr.addstr(height-3-i, 0, ' ' * (width-1))
        self.stdscr.addstr(height-3-i, 0, self.print_queue[len(self.print_queue) - i - 1])
    
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
          # resize the console based on terminal width and height mid loop
          height, width = self.stdscr.getmaxyx()
          curses.resizeterm(height, width)
          last_height = height
          last_width = width
        elif key == curses.KEY_UP:
          # iterate back through the command history if possible
          if len(self.command_history)-1>self.command_pointer:
            self.command_pointer = self.command_pointer + 1
            self.user_input = self.command_history[-self.command_pointer-1]
            self.command_length = len(self.user_input)
        elif key == curses.KEY_DOWN:
          # iterate back through the command history if possible
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
            #User typed the following key, so add it to input
            self.command_length = self.command_length + 1
            self.command_pointer = -1
            self.user_input += key.encode()
    except curses.error:
        # Ignore errors for now
        pass

    #Border between input an output sections of debug console
    self.stdscr.addstr(height-2, 0, '_'*(width-1))
    
    # Print input
    self.stdscr.addstr(height-1, 3, ' '*(width-4))
    self.stdscr.addstr(height-1, 3, self.user_input.decode())
    self.stdscr.addstr(height-1, self.command_length+3,'█')

    # Refresh screen
    self.stdscr.refresh()

  # Returns all the raw commands entered into the debug console
  def fetch_queued_commands(self):
    queue_to_return = self.command_queue.copy()
    self.command_queue.clear()
    return queue_to_return