# Please make sure you have acknowledged AurorOS license.
# It is available at https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md

import os
import re

allow_merging = True
was_there_error = False

def list_directory(path):
    structure = []

    for item in os.listdir(path):
        full_path = os.path.join(path, item)
        if os.path.isdir(full_path):
            structure.append({"name": item, "contents": list_directory(full_path)})
        elif item.endswith((".c", ".h")):
            structure.append(full_path)
    
    return structure

def check_trademark(files, trademark_text):
    global allow_merging, was_there_error

    for file in files:
        try:
            with open(file, "r", encoding="utf-8") as f:
                first_lines = f.read(len(trademark_text))
                if first_lines.strip() != trademark_text.strip():
                    print(f"❌ Missing trademark: {file}")
                    allow_merging = False
                else:
                    print(f"✅ OK: {file}")
        except Exception as e:
            print(f"⚠️ ERROR reading {file}: {e}")
            was_there_error = True

def check_naming_convention(files):
    global allow_merging

    camel_pascal_pattern = re.compile(r"\b[A-Z][a-z]+(?:[A-Z][a-z]*)*\b")

    for file in files:
        try:
            with open(file, "r", encoding="utf-8") as f:
                content = f.read()
                if camel_pascal_pattern.search(content):
                    print(f"❌ Forbidden naming convention found: {file}")
                    allow_merging = False
                else:
                    print(f"✅ Naming OK: {file}")
        except Exception as e:
            print(f"⚠️ ERROR reading {file}: {e}")
            was_there_error = True

def check_include_folder():
    global allow_merging
    include_path = os.path.join(base_path, "include")

    if os.path.exists(include_path) and os.path.isdir(include_path):
        for item in os.listdir(include_path):
            if not item.endswith(".h"):
                print(f"❌ Forbidden file in 'include/' folder: {item}")
                allow_merging = False
    else:
        print("✅ 'include/' folder check skipped (not found)")

def check_includes(files):
    global allow_merging

    include_pattern = re.compile(r'#include\s+"../include/([^"]+)"')

    for file in files:
        try:
            with open(file, "r", encoding="utf-8") as f:
                content = f.read()
                if include_pattern.search(content):
                    print(f"❌ Invalid include found (use <something.h> instead of \"../include/something.h\"): {file}")
                    allow_merging = False
        except Exception as e:
            print(f"⚠️ ERROR reading {file}: {e}")
            was_there_error = True

base_path = "." 

with open("trademark.txt", "r", encoding="utf-8") as f:
    trademark = f.read().strip()

directory_structure = list_directory(base_path)

files_to_check = []
def extract_files(structure):
    for item in structure:
        if isinstance(item, str):
            files_to_check.append(item)
        elif isinstance(item, dict):
            extract_files(item["contents"])

extract_files(directory_structure)

check_trademark(files_to_check, trademark)
check_naming_convention(files_to_check)
check_include_folder()
check_includes(files_to_check)

if was_there_error:
    print("[ ⚠️ RESULT ⚠️ ] Our guidelines verify tool cannot check if your code follows standards.")
elif allow_merging:
    print("[ ✅ RESULT ✅ ] Your code follows the guidelines of AurorOS!")
    print("[ ✅ RESULT ✅ ] Submit your code here: https://github.com/Interpuce/AurorOS/pulls")
    print("[ ✅ RESULT ✅ ] Your code still can be rejected for a lot of reasons.")
else:
    print("[ ❌ RESULT ❌ ] Your code does not follow the guidelines of AurorOS!")