#!/usr/bin/env python3
import sys

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.bin output.h")
        sys.exit(1)

    in_path, out_path = sys.argv[1], sys.argv[2]
    with open(in_path, "rb") as f:
        data = f.read()

    var_name = out_path.replace(".", "_").replace("/", "_")

    lines = []
    lines.append(f"static const unsigned char {var_name}[{len(data)}] = {{\n")

    for i, b in enumerate(data):
        lines.append(f"0x{b:02x},")
        if (i + 1) % 12 == 0:
            lines.append("\n")

    lines.append("\n};")
    
    with open(out_path, "w") as out:
        out.write("".join(lines))

if __name__ == "__main__":
    main()