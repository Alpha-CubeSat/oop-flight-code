class SFRLookup:
  def __init__(self, sfr_debug=False):
    self.debug = sfr_debug
    self.parse_sfr_fields()


  # Parses the sfr.cpp file for all the sfr fields building LUTs for both opcode
  # to sfr field name and sfr field name to opcode
  def parse_sfr_fields(self):
    d = {}
    d2 = {}
    path = 'src/sfr.cpp'
    if(self.debug):
      path = '../src/sfr.cpp'

    with open(path) as f:
      sfr_fields = []
      curr_namespace = ""
      for line in f:
        if "namespace" in line and "{" in line:
          components = line.split()
          if components[1] != curr_namespace:
            if len(sfr_fields) > 0:
              d[curr_namespace] = sfr_fields
            curr_namespace = components[1]
            sfr_fields = []
        if "SFRField" in line:
          components = line.split()
          mapping = {}
          op_code_location = line.index("0x")
          op_code = line[op_code_location:op_code_location + (line[op_code_location:].index(","))]
          mapping[components[1]] = op_code
          sfr_fields.append(mapping)
          d2[op_code] = curr_namespace + "." + components[1]
    self.sfr_to_opcode_lut = d
    self.opcode_to_sfr_lut = d2

  # gets opcode from sfr field name (Using the pre-built LUT)
  def get_opcode(self, sfr_field):
    try:
      [namespace, field] = sfr_field.split('.')
      fields = self.sfr_to_opcode_lut[namespace]
      for f in fields:
        if field in f.keys():
          return f[field]
    except:
      return None
    return None
  
  # gets sfr field name from opcode (Using the pre-built LUT)
  def get_sfr_field(self, opcode):
    try:
      return self.opcode_to_sfr_lut[opcode]
    except:
      return None
    return None

# Runs just the SFR Loader (Used for testing)
if __name__ == '__main__':
    lookup = SFRLookup()
    print(lookup.sfr_to_opcode_lut)
    print(lookup.opcode_to_sfr_lut)
