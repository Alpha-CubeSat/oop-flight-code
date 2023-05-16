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

# Determine which curses module to use based on OS (curses not supported on windows)
if platform.system() == 'Windows':
    import windows_curses as curses
else:
    import curses

def main(stdscr):
  # Loads all the Console Commands
  f = open('logger/console_commands.json', 'r')
  console_commands = json.load(f)
  f.close()

  plotter = Plotter()

  # Starts the Plotter on a different process so plotly dashboard is able to consider itself the main of the process
  plotter_process = multiprocessing.Process(target=plotter.run)
  plotter_process.start()

  print("STARTING DEBUG CONSOLE...")
  
  time.sleep(3)
  
  # Start
  logger = Logger(stdscr)
  serial_talker = SerialTalker(logger)

  commands_to_process = collections.deque()
  
  command_processor = CommandProcessor(console_commands)

  # Main Alpha Logger Loop
  while True:
    # Handle debug adds or removes
    for e in command_processor.get_adds():
      serial_talker.add_debug(e)
    for e in command_processor.get_removes():
      serial_talker.remove_debug(e)
    
    # gets the next serial message received that should be printed
    serial_output = serial_talker.read()
    if serial_output:
      # Print the output to the debug console
      logger.queue_output_for_printing(serial_output)

    # Fetch all the commands from the logger
    # Send all the commands to command processor to be processed
    # and send the cooresponding output back to the logger
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
    # Start everything with curses wrapper so curses handles the process.
    curses.wrapper(main)
