## Home work #8 (OTUS CPP DEVELOPER PROFESSIONAL)

Executable file **./bayan**

### Available options:
- **--help** - Produce help message
- **--scan-dirs** - List of directories to search duplicate files
- **--ignore-scan-dirs** - List of directories to ignore in the process of searching
- **--recursive-scan** - Recursive searching (disabled by default)
- **--min-file-size** - Minimal allowed file size to process, in bytes (1 byte by default)
- **--files-masks** - Files names masks to process (use '*' for any number of characters, '?' for one character)
- **--block-size** - Block size to read files, in bytes (1 byte by default)
- **--hash-func** - Hash function to compare files, "default" or "crc32" (boost::hash will be used when "default" enabled )

### Example of the command:

    ./bayan --scan-dirs "./otus-cpp-hw-8/" --ignore-scan-dirs "./otus-cpp-hw-8/.git/" --ignore-scan-dirs "./otus-cpp-hw-8/.idea/" --recursive-scan --block-size 5 --min-file-size 10 --hash-func "crc32" --files-masks "*.cpp" --files-masks "*.h"