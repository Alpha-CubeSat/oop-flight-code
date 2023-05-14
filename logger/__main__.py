import platform
import os
import time
import json

import collections

from .command_processor import Command, CommandProcessor
from .serial_talk import SerialTalker
from .logger import Logger
import multiprocessing
from .plotter import Plotter

if platform.system() == 'Windows':
    import windows_curses as curses
else:
    import curses

def main(stdscr):
  f = open('logger/console_commands.json', 'r')
  console_commands = json.load(f)
  f.close()

  plotter = Plotter()

  plotter_process = multiprocessing.Process(target=plotter.run)
  plotter_process.start()
  os.system('cls' if os.name == 'nt' else 'clear')
  print("STARTING DEBUG CONSOLE...")
  
  time.sleep(3)
  
  logger = Logger(stdscr)
  serial_talker = SerialTalker(logger)

  commands_to_process = collections.deque()
  
  command_processor = CommandProcessor(console_commands)
  while True:
    for e in command_processor.get_adds():
      serial_talker.add_debug(e)
    for e in command_processor.get_removes():
      serial_talker.remove_debug(e)
    # serial_talker.set_debug(command_processor.get_debug_toggle_status())
    serial_output = serial_talker.read()
    if serial_output:
      logger.queue_output_for_printing(serial_output)
    # logger.run_log_cycle()
    commands_to_process = logger.fetch_queued_commands()
    command_processor.push_debug_list(serial_talker.debug_starts)
    while len(commands_to_process) > 0:
      c = Command(commands_to_process.pop(), console_commands)
      outputs = command_processor.process_command(c)
      while len(command_processor.serial_messages) >0:
        logger.queue_output_for_printing("WRITE")
        serial_talker.write(command_processor.serial_messages.pop())
      if outputs:
        for output in outputs:
          logger.queue_output_for_printing(output)



  

if __name__ == '__main__':
    # Call curses.wrapper with the main function
    curses.wrapper(main)
