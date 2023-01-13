

namespace ng
{
    /*
     We need to make this instances static because we only need one of this things at any given time
     and not use constructors because static things get allocated before the main function runs, and constructors get executed
     in some random order we can't predict.
    */
    class FileLogger : public EngineSystem
    {
    public:
        FILE *logFile;
        static FileLogger *getInstance()
        {
            // this var gets created and initialize only on the first getInstance call
            static FileLogger *instance = nullptr;
            if (!instance)
            {
                instance = new FileLogger();
            }
            return instance;
        }

        uint32 startup()
        {
            time_t rawTime;
            struct tm *timeInfo;
            time(&rawTime);
            timeInfo = localtime(&rawTime);

            char fileName[50];
            strftime(fileName, sizeof(fileName), "%H-%M-%S-%F.txt", timeInfo);

            char filePath[150];
            strcat(filePath, "logs/");
            strcat(filePath, fileName);

            logFile = fopen(filePath, "a");
            if (logFile == NULL)
            {
                fputs("ERROR while opening log file\n", stderr);
                perror("Could not open logFile\n");
                return 1;
            }
            return 0;
        }

        void shutdown()
        {
            fclose(logFile);
        }

        void writeLog(const char *fmt, ...)
        {
            if (logFile == NULL)
            {
                printf("Log file is not open, did you forget to initialize it?");
                return;
            }
            char buffer[256];
            va_list args;
            va_start(args, fmt);
            uint32 total = vsprintf(buffer, fmt, args);

            if (fwrite(buffer, sizeof(char), total, logFile) != total)
            {
                fputs("Could not write the log\n", stderr);
                perror("Could not write the log\n");
            }

            fflush(logFile);
            va_end(args);
        }

    private:
        FileLogger(){};
        ~FileLogger(){};
    };

    class Logger : public EngineSystem
    {
    public:
        static Logger *getInstance()
        {
            // this var gets created and initialize only on the first getInstance call
            static Logger *instance = nullptr;
            if (!instance)
            {
                instance = new Logger(); // todo get memory from memorypool
            }
            return instance;
        }

        uint32 startup()
        {
            return 0;
        }

        void shutdown()
        {
            return;
        }

        void writeLog(const char *fmt, ...)
        {
            char buffer[256];
            va_list args;
            va_start(args, fmt);
            uint32 total = vsprintf(buffer, fmt, args);

            printf("%s", buffer);

            va_end(args);
        }

    private:
        Logger(){};
        ~Logger(){};
    };

    Randomizer dungeonRandomizer = getRandomInt(0, 100);

    DungeonRoom *dungeonRoomInit()
    {
        DungeonRoom *subRoom = (DungeonRoom *)malloc(sizeof(DungeonRoom));
        if (subRoom == NULL)
        {
            return NULL;
        }
        subRoom->subRoom = NULL;
        subRoom->sisterRoom = NULL;
        memset(subRoom, 0, sizeof(subRoom));
        return subRoom;
    }

    void
    createSubRooms(DungeonRoom *masterRoom, uint32 minRoomHeight, uint32 minRoomWidth)
    {
        Timer("createSubRooms");
        if (masterRoom->height > 2 * minRoomHeight && masterRoom->width > 2 * minRoomWidth)
        {
            DungeonRoom *subRoom1 = dungeonRoomInit();
            DungeonRoom *subRoom2 = dungeonRoomInit();
            if (subRoom1 == NULL || subRoom2 == NULL)
            {
                return;
            }

            // odd for vertical, even for horizontal
            int directionOfSplit = dungeonRandomizer.distribution.intD(dungeonRandomizer.generator);
            int guessTries = 10;
            if (directionOfSplit % 2 != 0)
            {
                while (guessTries >= 0)
                {
                    auto randomWidth = getRandomInt(minRoomWidth, masterRoom->width);
                    int pointOfSplit = randomWidth.distribution.intD(randomWidth.generator);
                    uint32 subRoom1Width = pointOfSplit;
                    uint32 subRoom2Width = masterRoom->width - pointOfSplit;
                    if (subRoom1Width < minRoomWidth || subRoom2Width < minRoomWidth) 
                    {
                        if (guessTries <= 0)
                        {
                            subRoom1Width = minRoomWidth;
                            subRoom2Width = minRoomWidth;
                        }
                        else
                        {
                            guessTries--;
                        }
                    }
                    else
                    {
                        guessTries = -1;

                        subRoom1->id = masterRoom->id + 1;
                        subRoom2->id = masterRoom->id + 1;

                        subRoom1->width = subRoom1Width;
                        subRoom2->width = subRoom2Width;

                        subRoom1->height = masterRoom->height;
                        subRoom2->height = masterRoom->height;

                        subRoom1->centerX = masterRoom->centerX - subRoom1->width / 2;
                        subRoom2->centerX = masterRoom->centerX + subRoom2->width / 2;

                        subRoom1->centerY = masterRoom->centerY;
                        subRoom2->centerY = masterRoom->centerY;

                        DungeonCorridor corridor = {};
                        corridor.startPos.x = subRoom1->centerX;
                        corridor.startPos.y = masterRoom->centerY;
                        corridor.endPos.x = subRoom2->centerX;
                        corridor.endPos.y = masterRoom->centerY;

                        subRoom1->corridor = corridor;
                        subRoom2->corridor = corridor;
                    }
                }
            }
            else
            {
                while (guessTries >= 0)
                {
                    auto randomHeight = getRandomInt(minRoomHeight, masterRoom->height);
                    int pointOfSplit = randomHeight.distribution.intD(randomHeight.generator);
                    uint32 subRoom1Height = pointOfSplit;
                    uint32 subRoom2Height = masterRoom->height - pointOfSplit;
                    if (subRoom1Height < minRoomHeight || subRoom2Height < minRoomHeight)
                    {
                        if (guessTries <= 0)
                        {
                            subRoom1Height = minRoomHeight;
                            subRoom2Height = minRoomHeight;
                        }
                        else
                        {
                            guessTries--;
                        }
                    }
                    else
                    {
                        guessTries = -1;

                        subRoom1->id = masterRoom->id + 1;
                        subRoom2->id = masterRoom->id + 1;

                        subRoom1->height = subRoom1Height;
                        subRoom2->height = subRoom2Height;

                        subRoom1->width = masterRoom->width;
                        subRoom2->width = masterRoom->width;

                        subRoom1->centerX = masterRoom->centerX;
                        subRoom2->centerX = masterRoom->centerX;

                        subRoom1->centerY = masterRoom->centerY - subRoom1->height / 2;
                        subRoom2->centerY = masterRoom->centerY + subRoom2->height / 2;

                        DungeonCorridor corridor = {};
                        corridor.startPos.x = masterRoom->centerX;
                        corridor.startPos.y = subRoom1->centerY;
                        corridor.endPos.x = masterRoom->centerX;
                        corridor.endPos.y = subRoom2->centerY;

                        subRoom1->corridor = corridor;
                        subRoom2->corridor = corridor;
                    }
                }
            }

            // if (subRoom1->width == 0 || subRoom2->width == 0)
            // {
            //     return;
            // }

            subRoom1->sisterRoom = subRoom2;
            // subRoom2->sisterRoom = subRoom1;
            masterRoom->subRoom = subRoom1;
            createSubRooms(subRoom1, minRoomHeight, minRoomWidth);
            createSubRooms(subRoom2, minRoomHeight, minRoomWidth);
        }

        return;
    }
    DungeonRoom *createDungeon(uint32 height, uint32 width, uint32 minRoomHeight, uint32 minRoomWidth)
    {
        Timer("createDungeon");
        // create rooms
        DungeonRoom *masterRoom = dungeonRoomInit();
        masterRoom->id = 0;
        masterRoom->height = height;
        masterRoom->width = width;
        masterRoom->centerX = width / 2;
        masterRoom->centerY = height / 2;
        masterRoom->sisterRoom = nullptr;
        masterRoom->subRoom = nullptr;
        createSubRooms(masterRoom, minRoomHeight, minRoomWidth);
        return masterRoom;
    }
}
/*
 ____
\____\
*/