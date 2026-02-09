import json
import subprocess
import sys
import glob

def build(config_file="build.json"):
    with open(config_file, "r") as f:
        config = json.load(f)

    compiler = config.get("compiler", "g++")
    
    # Get local project files
    local_sources = glob.glob("*.cpp")
    
    # Get external library files from JSON
    extra_sources = config.get("extraSources", [])
    
    # Merge and quote everything
    all_sources = local_sources + extra_sources
    source_str = " ".join(f"\"{f}\"" for f in all_sources)
    
    include_paths = " ".join(f"-I\"{path}\"" for path in config.get("includePaths", []))
    library_paths = " ".join(f"-L\"{path}\"" for path in config.get("libraryPaths", []))
    libraries = " ".join(f"-l{lib}" for lib in config.get("libraries", []))
    flags = " ".join(config.get("compilerFlags", []))
    output = config.get("output", "main.exe")

    command = f"{compiler} {source_str} {include_paths} {flags} {library_paths} {libraries} -o {output}"
    
    print(f"Build Command: {command}\n")

    try:
        result = subprocess.run(command, capture_output=True, text=True, shell=True)
        if result.returncode == 0:
            print(f"✅ Success: {output}")
        else:
            print("❌ Failure:")
            print(result.stderr)
    except Exception as e:
        print(f"System Error: {e}")

if __name__ == "__main__":
    cfg = sys.argv[1] if len(sys.argv) > 1 else "build.json"
    build(cfg)
