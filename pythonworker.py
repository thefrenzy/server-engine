import json
import subprocess
import sys
import glob

def build(config_file="build.json"):
    with open(config_file, "r") as f:
        config = json.load(f)

    compiler = config.get("compiler", "g++")
    
    # Grab all source files automatically
    all_cpp_files = glob.glob("*.cpp")
    source_files = " ".join(all_cpp_files)
    
    include_paths = " ".join(f"-I\"{path}\"" for path in config.get("includePaths", []))
    library_paths = " ".join(f"-L\"{path}\"" for path in config.get("libraryPaths", []))
    libraries = " ".join(f"-l{lib}" for lib in config.get("libraries", []))
    compiler_flags = " ".join(config.get("compilerFlags", []))
    output = config.get("output", "network_engine.exe")

    # CRITICAL FIX: Move {libraries} to the very end of the command
    command = f"{compiler} {source_files} {include_paths} {compiler_flags} {library_paths} {libraries} -o {output}"
    
    print(f"Compiling: {all_cpp_files}")
    print(f"Command: {command}\n")

    try:
        # We use capture_output=True so we can actually see WHY it failed
        result = subprocess.run(command, capture_output=True, text=True, shell=True)
        if result.returncode == 0:
            print(f"✅ Build successful: {output}")
        else:
            print("❌ Build failed. Error Details:")
            print(result.stderr) # This tells you exactly what is wrong
    except Exception as e:
        print(f"System Error: {e}")

if __name__ == "__main__":
    cfg = sys.argv[1] if len(sys.argv) > 1 else "build.json"
    build(cfg)
