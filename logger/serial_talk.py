import serial
import os
import threading
import collections

class SerialTalker:
  def __init__(self, logger):
    self.stop = False
    self.port = '/dev/ttyACM0'
    self.baud = 9600

    self.debug = False

    self.write_queue = collections.deque()
    self.read_queue = collections.deque()

    try:
      self.ser = serial.Serial(self.port, self.baud)
      self.write_thread = threading.Thread(target=self.talk_to_sat)
      self.write_thread.daemon = True
      self.write_thread.start()
      self.read_thread = threading.Thread(target=self.listen_to_sat)
      self.read_thread.daemon = True
      self.read_thread.start()
    except:
      pass

    self.debug_starts = set([])
    self.logger = logger
    

  def stop_serial_connection(self):
    self.stop = True
    self.ser.cancel_read()
    self.ser.cancel_write()
    self.ser.close()

  def restart_serial_connection(self):
    self.stop = False
    self.ser = serial.Serial(self.port, self.baud)

  def listen_to_sat(self):
    while not self.stop:
      if self.ser.isOpen():
        line = self.ser.readline().rstrip().decode('utf-8')
        if "all" in self.debug_starts:
          self.read_queue.append(line)
        for start in self.debug_starts:
          if(line.lower().startswith(start)):
            self.read_queue.append(line)
        if(line.lower().startswith("response")):
          self.read_queue.append(line)

  def talk_to_sat(self):
    while not self.stop:
      if(len(self.write_queue)>0):
        if self.ser.isOpen():
          self.logger.queue_output_for_printing("SENDING")
          self.ser.write(self.write_queue.pop().encode('utf-8'))

  def write(self, message):
    self.write_queue.append(message)

  def read(self):
    if(len(self.read_queue)>0):
      return self.read_queue.pop()

  def add_debug(self, debug_group):
    self.debug_starts.add(debug_group)
  def remove_debug(self, debug_group):
    if debug_group in self.debug_starts:
      self.debug_starts.remove(debug_group)
