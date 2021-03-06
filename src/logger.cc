#include "logger.hh"
#include "common.hh"
#include <ctime>
#include <windows.h>

#ifndef NDEBUG
#define RAINBOMIZER_BUILD "Debug Build: " __DATE__ " " __TIME__
#else
#define RAINBOMIZER_BUILD "Build v1.0.2: " __DATE__ " " __TIME__
#endif

namespace Rainbomizer {

/*******************************************************/
std::string
GetTimeNow ()
{
    time_t currentTime;
    char   str[256];

    time (&currentTime);

    auto tm = std::localtime (&currentTime);
    sprintf (str, "%02d-%02d-%04d", tm->tm_mday, tm->tm_mon + 1,
             1900 + tm->tm_year);

    return str;
}

/*******************************************************/
FILE *
Logger::GetLogFile ()
{
    if (!mFile)
        {
            mFile = Common::GetRainbomizerFile (GetTimeNow () + ".txt", "a+",
                                                "logs/");
            if (!mFile)
                {
                    MessageBox (NULL, "Unable to open log file", "Error",
                                MB_ICONHAND);
                    mFile = stdout;
                }

            fprintf (mFile, "===========================================\n");
            fprintf (mFile, "%d\n", (int) time (NULL));
            fprintf (mFile, "Rainbomizer IV/EFLC Build: %s \n",
                     RAINBOMIZER_BUILD);
            fprintf (mFile, "===========================================\n");
        }

    return mFile;
}

/*******************************************************/
void
Logger::LogMessage (const char *format, ...)
{
    FILE *file = GetLogFile ();
    fprintf (file, "[%d]: ", int (time (NULL)));

    va_list args;
    va_start (args, format);
    vfprintf (file, format, args);
    va_end (args);

    fputc ('\n', file);
    fflush (file);
}

FILE *Logger::mFile = nullptr;

} // namespace Rainbomizer
