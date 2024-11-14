import re

def extract_hex_strings(input_file, output_file):
    with open(input_file, 'r') as file:
        content = file.read()

    # Regex to find "Image:" followed by hex string
    pattern = r'Image:\s*([0-9A-Fa-f]+)'

    # Find all matches
    hex_strings = re.findall(pattern, content)

    # Join all hex strings together and remove any whitespace characters
    combined_hex = ''.join(hex_strings).replace("\n", "")

    # Write the cleaned hex string to the output file without additional whitespace
    with open(output_file, 'w') as file:
        file.write(combined_hex)

input_file = 'firstphoto.txt'  # Replace with desired path
output_hex_file = 'img1_.txt'  # Replace with desired path

extract_hex_strings(input_file, output_hex_file)
print(f"Hex strings extracted, cleaned, and saved to {output_hex_file}.")

