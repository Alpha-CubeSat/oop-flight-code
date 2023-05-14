if __name__ == "__main__":
  from load_sfr import SFRLookup
  from serial_talk import SerialTalker
else:
  from .load_sfr import SFRLookup
  from .serial_talk import SerialTalker
import json
import collections

class Command:
  def __init__(self, command_def, commands):
    self.commands = commands

    command_def = str(command_def.decode('utf-8'))
    command_def = command_def.split(" ")
    for i in range(len(command_def)):
      command_def[i] = command_def[i].lower()
    self.command_type = "unknown"
    self.params = ["invalid","invalid"]
    num_parts = len(command_def)

    if num_parts > 0:
      for command in commands['commands']:
        # print(command)
        if command["name"] == command_def[0]:
          self.command_type = command["name"]
          if (num_parts - 1) <= (len(command["params"])):
            curr_param = 0
            for param in command['params']:
              # print(curr_param)
              cont = True
              if num_parts <= curr_param + 1:
                cont = False

              if cont:
                if param['options'] == 'sfr_fields':
                  self.params[curr_param] = command_def[1+curr_param]
                elif command_def[1+curr_param] in param['options']:
                  self.params[curr_param] = command_def[1+curr_param]
              curr_param = curr_param + 1

  
  def getCommmandType(self):
    return self.command_type
  def getParam1(self):
    return self.params[0]
  def getParam2(self):
    return self.params[1]
  
class CommandProcessor:
  def __init__(self, commands, cp_debug = False):
    lookup_tables = SFRLookup(sfr_debug=cp_debug)
    self.lookup_tables = lookup_tables
    self.commands = commands
    self.current_debug_list = set([])
    self.to_add = set([])
    self.to_remove = set([])
    self.serial_messages = collections.deque()

  def get_debug_toggle_status(self):
    return self.debug_toggled
  
  def get_adds(self):
    s = self.to_add.copy()
    self.to_add.clear()
    return s
  
  def get_removes(self):
    s = self.to_remove.copy()
    self.to_add.clear()
    return s




  def process_command(self, command):
    response = []
    # print(command.getCommmandType())
    # print(command.getParam1())
    # print(command.getParam2())
    if(command.getCommmandType() == "read"):
      if command.getParam1() == "invalid":
        response.append("Please specify which SFRField to read.")
      else:
        opcode = self.lookup_tables.get_opcode(command.getParam1())
        if opcode:
          response.append("Fetching " + command.getParam1() + " (opcode " + opcode + ")")
          self.serial_messages.append("debug_console.read."+opcode)
        else:
          response.append(command.getParam1() + " is not a valid SFRField")
    elif(command.getCommmandType() == "uplink"):
      if command.getParam1() == "invalid":
        response.append("Invalid Uplink Command")
      elif command.getParam1() == "sfr_override":
        if command.getParam2() == "invalid":
          response.append("please provide an SFRField to override and a value")
        else:
          opcode = self.lookup_tables.get_opcode(command.getParam1())
          if opcode:
            response.append("Uplinking " + command.getParam1() + " (opcode " + opcode + ")" + " - TODO: ADD VALUE PARAM")
          else:
            response.append(command.getParam2() + " is not a valid SFRField")
    elif(command.getCommmandType() == "log"):
      if command.getParam1() == "invalid":
        response.append("Please specify which SFRField to log and plot.")
      else:
        opcode = self.lookup_tables.get_opcode(command.getParam1())
        if opcode:
          response.append("Logging " + command.getParam1() + " (opcode " + opcode + ")")
        else:
          response.append(command.getParam1() + " is not a valid SFRField")
    elif(command.getCommmandType() == "debug"):
      if command.getParam1() == "list":
        l = ""
        for item in self.current_debug_list:
          l = l + item + ', '
        if l == "":
          response.append("No debugs on")
        else:
          response.append("Debugs on: " + l[:-2])
      elif(command.getParam2() == "invalid"):
        response.append("Invalid command. Please try again.")
      elif command.getParam1() == "add":
        self.to_add.add(command.getParam2())
        response.append("Added " + command.getParam2() + " to debug")

      elif command.getParam1() == "remove":
        self.to_remove.add(command.getParam2())
        response.append("Removed " + command.getParam2() + " from debug")
        
    elif(command.getCommmandType() == "help"):
      if command.getParam1() == "invalid":
        response.append("~~~ WELCOME TO ALPHA LOGGER TEST UTILITY ~~~")
        response.append("AlphaLogger enables live testing and debugging of the satellite.")
        response.append("Use 'help [cmd]' to find out more about each supported command and its accepted arguments.")
        the_options = "Commands: ["
        for command_info in self.commands['commands']:
          the_options = the_options + command_info["name"] + ", "
        the_options = the_options[:-2] + "]"
        response.append(the_options)
      else:
        command_info = {}
        for command_info_option in self.commands['commands']:
          if command.getParam1() == command_info_option['name']:
            command_info = command_info_option
            break
        response.append("~~~ " + command_info["name"].upper() + " COMMAND HELP ~~~")
        response.append("Description: " + command_info["description"])
        usage = command_info["name"]
        for param in command_info['params']:
          usage = usage + " ["+ param['name'] +"]"
        response.append("Usage: " + usage)
        for param in command_info['params']:
          response.append(param['name'] + " options: " + str(param["options"]))

    elif(command.getCommmandType() == "exit"):
      quit()
    else:
      response.append("Command Invalid. Please enter a valid command.")

    return response

  def push_debug_list(self, debug_list):
    self.current_debug_list = debug_list

if __name__ == '__main__':
    f = open('console_commands.json', 'r')
    console_commands = json.load(f)
    f.close()
    command_processor = CommandProcessor(console_commands, cp_debug=True)
    i = input()
    c = Command(i.encode('utf-8'), console_commands)
    r = command_processor.process_command(c)
    print(r)
