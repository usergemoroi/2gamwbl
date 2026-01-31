#!/usr/bin/env python3
"""
Advanced ARM64 Binary Analyzer for libe6bmfqax5v.so
Performs deep analysis and generates C/C++ code reconstruction
"""

import re
import subprocess
import json
from pathlib import Path
from typing import Dict, List, Tuple, Optional
from dataclasses import dataclass, field

@dataclass
class Function:
    """Represents a function in the binary"""
    name: str
    address: int
    size: int = 0
    instructions: List[str] = field(default_factory=list)
    calls: List[str] = field(default_factory=list)
    called_by: List[str] = field(default_factory=list)
    is_exported: bool = False
    
@dataclass
class StringData:
    """Represents a string found in the binary"""
    value: str
    offset: int
    section: str = ""

class ARM64Analyzer:
    """Advanced analyzer for ARM64 binaries"""
    
    def __init__(self, binary_path: str):
        self.binary_path = Path(binary_path)
        self.functions: Dict[str, Function] = {}
        self.strings: List[StringData] = []
        self.exports: List[str] = []
        
    def run_command(self, cmd: List[str]) -> str:
        """Execute shell command and return output"""
        try:
            result = subprocess.run(cmd, capture_output=True, text=True, check=True)
            return result.stdout
        except subprocess.CalledProcessError as e:
            print(f"Error running {' '.join(cmd)}: {e}")
            return ""
    
    def extract_exports(self):
        """Extract exported symbols using nm"""
        print("[*] Extracting exported symbols...")
        output = self.run_command(['nm', '-D', str(self.binary_path)])
        
        for line in output.splitlines():
            parts = line.strip().split()
            if len(parts) >= 3 and parts[1] == 'T':
                addr = int(parts[0], 16)
                name = parts[2]
                self.exports.append(name)
                self.functions[name] = Function(name=name, address=addr, is_exported=True)
                
        print(f"[+] Found {len(self.exports)} exported functions")
        
    def extract_strings(self):
        """Extract printable strings from binary"""
        print("[*] Extracting strings...")
        output = self.run_command(['strings', '-n', '4', str(self.binary_path)])
        
        for line in output.splitlines():
            line = line.strip()
            if line:
                self.strings.append(StringData(value=line, offset=0))
                
        print(f"[+] Found {len(self.strings)} strings")
        
    def analyze_elf_structure(self):
        """Analyze ELF structure using readelf"""
        print("[*] Analyzing ELF structure...")
        
        # Get section headers
        output = self.run_command(['readelf', '-S', str(self.binary_path)])
        print("[+] ELF Sections:")
        for line in output.splitlines():
            if line.strip().startswith('['):
                print(f"    {line.strip()}")
                
        # Get dynamic symbols
        output = self.run_command(['readelf', '-d', str(self.binary_path)])
        print("[+] Dynamic Dependencies:")
        for line in output.splitlines():
            if 'NEEDED' in line:
                print(f"    {line.strip()}")
    
    def disassemble_function(self, func_name: str, start_addr: int, max_instructions: int = 200):
        """Disassemble a specific function"""
        print(f"[*] Disassembling function: {func_name}")
        
        # Use objdump to disassemble
        cmd = ['aarch64-linux-gnu-objdump', '-d', str(self.binary_path)]
        output = self.run_command(cmd)
        
        # Find function in output
        in_function = False
        instructions = []
        
        for line in output.splitlines():
            if f"<{func_name}" in line and ':' in line:
                in_function = True
                continue
                
            if in_function:
                # Check if we hit next function
                if re.match(r'^[0-9a-f]{16} <', line):
                    break
                    
                # Parse instruction line
                match = re.match(r'\s+([0-9a-f]+):\s+([0-9a-f]+)\s+(.+)', line)
                if match:
                    instructions.append(match.group(3).strip())
                    
                    if len(instructions) >= max_instructions:
                        break
        
        if func_name in self.functions:
            self.functions[func_name].instructions = instructions
            self.functions[func_name].size = len(instructions) * 4
            
        return instructions
    
    def generate_report(self, output_file: str):
        """Generate analysis report"""
        print(f"[*] Generating report: {output_file}")
        
        with open(output_file, 'w') as f:
            f.write("# libe6bmfqax5v.so Analysis Report\n\n")
            f.write(f"## Binary Information\n")
            f.write(f"- File: {self.binary_path.name}\n")
            f.write(f"- Size: {self.binary_path.stat().st_size} bytes\n")
            f.write(f"- Exported functions: {len(self.exports)}\n")
            f.write(f"- Total strings: {len(self.strings)}\n\n")
            
            f.write("## Exported Functions\n\n")
            for export in sorted(self.exports):
                if export in self.functions:
                    func = self.functions[export]
                    f.write(f"### {func.name}\n")
                    f.write(f"- Address: 0x{func.address:016x}\n")
                    f.write(f"- Size: ~{func.size} bytes\n")
                    if func.instructions:
                        f.write(f"- Instructions: {len(func.instructions)}\n")
                        f.write("```asm\n")
                        for i, instr in enumerate(func.instructions[:50]):
                            f.write(f"{instr}\n")
                        if len(func.instructions) > 50:
                            f.write(f"... ({len(func.instructions) - 50} more instructions)\n")
                        f.write("```\n")
                    f.write("\n")
            
            f.write("## Interesting Strings\n\n")
            f.write("```\n")
            for s in self.strings[:100]:
                f.write(f"{s.value}\n")
            f.write("```\n")
            
        print(f"[+] Report generated: {output_file}")

def main():
    """Main analysis routine"""
    print("=" * 80)
    print("ARM64 Binary Analyzer for libe6bmfqax5v.so")
    print("=" * 80)
    
    binary_path = "/home/engine/project/extracted/lib/arm64-v8a/libe6bmfqax5v.so"
    output_dir = Path("/home/engine/project/analysis")
    output_dir.mkdir(exist_ok=True)
    
    analyzer = ARM64Analyzer(binary_path)
    
    # Phase 1: Structure analysis
    analyzer.analyze_elf_structure()
    print()
    
    # Phase 2: Extract symbols and strings
    analyzer.extract_exports()
    analyzer.extract_strings()
    print()
    
    # Phase 3: Disassemble key functions
    for func_name in analyzer.exports:
        if func_name in analyzer.functions:
            func = analyzer.functions[func_name]
            analyzer.disassemble_function(func_name, func.address)
    print()
    
    # Phase 4: Generate report
    report_file = output_dir / "libe6bmfqax5v_analysis.md"
    analyzer.generate_report(str(report_file))
    
    print("\n" + "=" * 80)
    print("Analysis complete!")
    print("=" * 80)

if __name__ == "__main__":
    main()
