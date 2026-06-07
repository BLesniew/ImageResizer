#include "CliParser.hpp"

#include <unistd.h>

CliParser::CliParser(int argc, char *argv[])
{
    if (argc < 3)
    {
        isValid = false;
    }

    for (;;)
    {
        switch (getopt(argc, argv, "i:o:e:c"))
        {
        case 'i':
            inputFilePath = optarg;
            continue;

        case 'o':
            outputFilePath = optarg;
            continue;

        case 'e':
        {
            allowedExtensions.clear();
            int i = 0;
            char extension = optarg[0];
            while (extension != '\0')
            {
                extension = optarg[i++];
                switch (extension)
                {
                case 'j':
                case 'J':
                    allowedExtensions.emplace(std::string{JPG_EXT});
                    break;

                case 'p':
                case 'P':
                    allowedExtensions.emplace(std::string{PNG_EXT});
                    break;

                case 't':
                case 'T':
                    allowedExtensions.emplace(std::string{TIFF_EXT});
                    break;

                default:
                    isValid = false;
                    return;
                }
            }

            continue;
        }

        case 'c':
            isCompressionAllowed = false;
            continue;

        default:
            isValid = false;
            return;

        case -1:
            break;
        }

        break;
    }

    if (inputFilePath.empty())
    {
        isValid = false;
        return;
    }
}