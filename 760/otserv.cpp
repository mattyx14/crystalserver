//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// otserv main. The only place where things get instantiated.
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////
#include "otpch.h"

#include "definitions.h"
#include <boost/asio.hpp>
#include "server.h"

#include <string>
#include <iostream>
#include <iomanip>

#include "otsystem.h"
#include "networkmessage.h"
#include "protocolgame.h"

#include <stdlib.h>
#include <time.h>
#include "game.h"

#include "iologindata.h"

#if !defined(__WINDOWS__)
#include <signal.h> // for sigemptyset()
#endif

#include "status.h"
#include "monsters.h"
#include "commands.h"
#include "outfit.h"
#include "vocation.h"
#include "scriptmanager.h"
#include "configmanager.h"

#include "tools.h"
#include "ban.h"

#include "resources.h"

#ifdef __OTSERV_ALLOCATOR__
#include "allocator.h"
#endif

#ifdef BOOST_NO_EXCEPTIONS
	#include <exception>
	void boost::throw_exception(std::exception const & e)
	{
		std::cout << "Boost exception: " << e.what() << std::endl;
	}
#endif

IPList serverIPs;

Ban g_bans;
Game g_game;
Commands commands;
Npcs g_npcs;
ConfigManager g_config;
Monsters g_monsters;
Vocations g_vocations;

Server* g_server = NULL;

OTSYS_THREAD_LOCKVAR g_loaderLock;
OTSYS_THREAD_SIGNALVAR g_loaderSignal;

#ifdef __EXCEPTION_TRACER__
#include "exception.h"
#endif
#include "networkmessage.h"

void startupErrorMessage(std::string errorStr)
{
	if(errorStr.length() > 0)
		std::cout << ":: ERROR: " << errorStr << std::endl;

	#ifdef WIN32
	system("pause");
	#else
	getchar();
	#endif
	exit(-1);
}

void mainLoader(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	#ifdef __OTSERV_ALLOCATOR_STATS__
	OTSYS_CREATE_THREAD(allocatorStatsThread, NULL);
	#endif

	#ifdef __EXCEPTION_TRACER__
	ExceptionHandler mainExceptionHandler;
	mainExceptionHandler.InstallHandler();
	#endif

        // ignore sigpipe...
        #ifdef WIN32
        //nothing yet
        #else
        struct sigaction sigh;
        sigh.sa_handler = SIG_IGN;
        sigh.sa_flags = 0;
        sigemptyset(&sigh.sa_mask);
        sigaction(SIGPIPE, &sigh, NULL);
        #endif

	OTSYS_THREAD_LOCKVARINIT(g_loaderLock);
	OTSYS_THREAD_SIGNALVARINIT(g_loaderSignal);

	Dispatcher::getDispatcher().addTask(createTask(boost::bind(mainLoader, argc, argv)));

	OTSYS_THREAD_WAITSIGNAL(g_loaderSignal, g_loaderLock);

	Server server(INADDR_ANY, g_config.getNumber(ConfigManager::PORT));
	std::cout << ":: " << g_config.getString(ConfigManager::SERVER_NAME) << " Server Online!" << std::endl << std::endl;
	g_server = &server;
	server.run();

#ifdef __EXCEPTION_TRACER__
	mainExceptionHandler.RemoveHandler();
#endif

	return 0;
}

void mainLoader(int argc, char *argv[])
{
	//dispatcher thread
	g_game.setGameState(GAME_STATE_STARTUP);

	srand((unsigned int)OTSYS_TIME());

	#ifdef WIN32
    	SetConsoleTitle(STATUS_SERVER_NAME);
	#endif

	std::cout << STATUS_SERVER_NAME << " - Version " << STATUS_SERVER_VERSION << "." << std::endl;
	std::cout << "A server developed by Tryller." << std::endl;
	std::cout << "Visit: https://github.com/tryller/crystalserver." << std::endl;
	std::cout << std::endl;

	// read global config
	std::cout << ":: Loading config" << std::endl;
	if(!g_config.loadFile("config.lua"))
    {
		startupErrorMessage("Unable to load config.lua!");
		return;
	}

	#ifdef WIN32
	std::string defaultPriority = asLowerCaseString(g_config.getString(ConfigManager::DEFAULT_PRIORITY));
	if(defaultPriority == "realtime")
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	else if(defaultPriority == "high")
  	 	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
  	else if(defaultPriority == "higher")
  		SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

	std::ostringstream mutexName;
	mutexName << "crystalserver_" << g_config.getNumber(ConfigManager::PORT);
	CreateMutex(NULL, FALSE, mutexName.str().c_str());
	if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
		startupErrorMessage("Another instance of the Crystal Server is already running with the same login port, please shut it down first or change ports for this one.");
		return;
	}
  	#endif

	std::cout << ":: Testing SQL connection... ";
	#if defined __USE_MYSQL__ && defined __USE_SQLITE__
	std::string sqlType = asLowerCaseString(g_config.getString(ConfigManager::SQL_TYPE));
	if(sqlType == "mysql")
	{
		g_config.setNumber(ConfigManager::SQLTYPE, SQL_TYPE_MYSQL);
		std::cout << "MySQL." << std::endl;
		Database* db = Database::getInstance();
		if(!db->connect())
			startupErrorMessage("Failed to connect to database, read doc/MYSQL_HELP for information or try SqLite which doesn't require any connection.");
	}
	else if(sqlType == "sqlite")
	{
		g_config.setNumber(ConfigManager::SQLTYPE, SQL_TYPE_SQLITE);
		std::cout << "SqLite." << std::endl;
		FILE* sqliteFile = fopen(g_config.getString(ConfigManager::SQLITE_DB).c_str(), "r");
		if(sqliteFile == NULL)
			startupErrorMessage("Failed to connect to sqlite database file, make sure it exists and is readable.");
		fclose(sqliteFile);
	}
	else
		startupErrorMessage("Unkwown sqlType, valid sqlTypes are: 'mysql' and 'sqlite'.");
	#elif defined __USE_MYSQL__
	std::cout << "MySQL." << std::endl;
	Database* db = Database::getInstance();
	if(!db->connect())
		startupErrorMessage("Failed to connect to database, read doc/MYSQL_HELP for information or try SqLite which doesn't require any connection.");
	#elif defined __USE_SQLITE__
	std::cout << "SqLite." << std::endl;
	FILE* sqliteFile = fopen(g_config.getString(ConfigManager::SQLITE_DB).c_str(), "r");
	if(sqliteFile == NULL)
		startupErrorMessage("Failed to connect to sqlite database file, make sure it exists and is readable.");
	fclose(sqliteFile);
	#else
	startupErrorMessage("Unknown sqlType... terminating!");
	#endif

	//load bans
	std::cout << ":: Loading bans" << std::endl;
	g_bans.init();
	if(!g_bans.loadBans())
		startupErrorMessage("Unable to load bans!");

	//load vocations
	std::cout << ":: Loading vocations" << std::endl;
	if(!g_vocations.loadFromXml())
		startupErrorMessage("Unable to load vocations!");

	//load commands
	std::cout << ":: Loading commands" << std::endl;
	if(!commands.loadFromXml())
		startupErrorMessage("Unable to load commands!");

	// load item data
	std::cout << ":: Loading items" << std::endl;
	if(Item::items.loadFromOtb("data/items/items.otb"))
		startupErrorMessage("Unable to load items (OTB)!");

	if(!Item::items.loadFromXml())
		startupErrorMessage("Unable to load items (XML)!");

	std::cout << ":: Loading script systems" << std::endl;
	if(!ScriptingManager::getInstance()->loadScriptSystems())
		startupErrorMessage("");

	std::cout << ":: Loading monsters" << std::endl;
	if(!g_monsters.loadFromXml())
		startupErrorMessage("Unable to load monsters!");

	std::cout << ":: Loading outfits" << std::endl;
	Outfits* outfits = Outfits::getInstance();
	if(!outfits->loadFromXml())
		startupErrorMessage("Unable to load outfits!");

	std::cout << ":: Loading experience stages" << std::endl;
	if(!g_game.loadExperienceStages())
		startupErrorMessage("Unable to load experience stages!");

	std::string passwordType = asLowerCaseString(g_config.getString(ConfigManager::PASSWORDTYPE));
	if(passwordType == "md5")
	{
		g_config.setNumber(ConfigManager::PASSWORD_TYPE, PASSWORD_TYPE_MD5);
		std::cout << ":: Using MD5 passwords" << std::endl;
	}
	else if(passwordType == "sha1")
	{
		g_config.setNumber(ConfigManager::PASSWORD_TYPE, PASSWORD_TYPE_SHA1);
		std::cout << ":: Using SHA1 passwords" << std::endl;
	}
	else
	{
		g_config.setNumber(ConfigManager::PASSWORD_TYPE, PASSWORD_TYPE_PLAIN);
		std::cout << ":: Using plaintext passwords" << std::endl;
	}

	std::cout << ":: Checking world type... ";
	std::string worldType = asLowerCaseString(g_config.getString(ConfigManager::WORLD_TYPE));
	if(worldType == "pvp")
		g_game.setWorldType(WORLD_TYPE_PVP);
	else if(worldType == "no-pvp")
		g_game.setWorldType(WORLD_TYPE_NO_PVP);
	else if(worldType == "pvp-enforced")
		g_game.setWorldType(WORLD_TYPE_PVP_ENFORCED);
	else
	{
		std::cout << std::endl << ":: ERROR: Unknown world type: " << g_config.getString(ConfigManager::WORLD_TYPE) << std::endl;
		startupErrorMessage("");
	}
	std::cout << asUpperCaseString(worldType) << std::endl;

	Status* status = Status::getInstance();
	status->setMaxPlayersOnline(g_config.getNumber(ConfigManager::MAX_PLAYERS));
	status->setMapAuthor(g_config.getString(ConfigManager::MAP_AUTHOR));
	status->setMapName(g_config.getString(ConfigManager::MAP_NAME));

	std::cout << ":: Loading map" << std::endl;
	if(!g_game.loadMap(g_config.getString(ConfigManager::MAP_NAME)))
		startupErrorMessage("");

	std::cout << ":: Initializing the game world!" << std::endl;
	g_game.setGameState(GAME_STATE_INIT);

	g_game.timedHighscoreUpdate();

	int32_t autoSaveEachMinutes = g_config.getNumber(ConfigManager::AUTO_SAVE_EACH_MINUTES);
	if(autoSaveEachMinutes > 0)
		Scheduler::getScheduler().addEvent(createSchedulerTask(autoSaveEachMinutes * 1000 * 60, boost::bind(&Game::autoSave, &g_game)));

	if(g_config.getString(ConfigManager::SERVERSAVE_ENABLED) == "yes" && g_config.getNumber(ConfigManager::SERVERSAVE_H) >= 0 && g_config.getNumber(ConfigManager::SERVERSAVE_H) <= 24)
	{
		int32_t prepareServerSaveHour = g_config.getNumber(ConfigManager::SERVERSAVE_H) - 1;
		int32_t hoursLeft = 0, minutesLeft = 0, minutesToRemove = 0;
		bool ignoreEvent = false;
		time_t timeNow = time(NULL);
		const tm* theTime = localtime(&timeNow);
		if(theTime->tm_hour > prepareServerSaveHour)
		{
			hoursLeft = 24 - (theTime->tm_hour - prepareServerSaveHour);
			if(theTime->tm_min > 55 && theTime->tm_min <= 59)
				minutesToRemove = theTime->tm_min - 55;
			else
				minutesLeft = 55 - theTime->tm_min;
		}
		else if(theTime->tm_hour == prepareServerSaveHour)
		{
			if(theTime->tm_min >= 55 && theTime->tm_min <= 59)
			{
				if(theTime->tm_min >= 57)
					g_game.setServerSaveMessage(0, true);

				if(theTime->tm_min == 59)
					g_game.setServerSaveMessage(1, true);

				g_game.prepareServerSave();
				ignoreEvent = true;
			}
			else
				minutesLeft = 55 - theTime->tm_min;
		}
		else
		{
			hoursLeft = prepareServerSaveHour - theTime->tm_hour;
			if(theTime->tm_min > 55 && theTime->tm_min <= 59)
				minutesToRemove = theTime->tm_min - 55;
			else
				minutesLeft = 55 - theTime->tm_min;
		}

		int32_t hoursLeftInMS = 60000 * 60 * hoursLeft;
		uint32_t minutesLeftInMS = 60000 * (minutesLeft - minutesToRemove);
		if(!ignoreEvent && (hoursLeftInMS + minutesLeftInMS) > 0)
			Scheduler::getScheduler().addEvent(createSchedulerTask(hoursLeftInMS + minutesLeftInMS, boost::bind(&Game::prepareServerSave, &g_game)));
	}

	std::cout << ":: Crystal Server starting up..." << std::endl;

	std::pair<uint32_t, uint32_t> IpNetMask;
	IpNetMask.first  = inet_addr("127.0.0.1");
	IpNetMask.second = 0xFFFFFFFF;
	serverIPs.push_back(IpNetMask);

	char szHostName[128];
	if(gethostname(szHostName, 128) == 0)
	{
		hostent *he = gethostbyname(szHostName);
		if(he)
		{
			unsigned char** addr = (unsigned char**)he->h_addr_list;
			while(addr[0] != NULL)
			{
				IpNetMask.first  = *(uint32_t*)(*addr);
				IpNetMask.second = 0x0000FFFF;
				serverIPs.push_back(IpNetMask);
				addr++;
			}
		}
	}
	std::string ip;
	ip = g_config.getString(ConfigManager::IP);

	uint32_t resolvedIp = inet_addr(ip.c_str());
	if(resolvedIp == INADDR_NONE)
	{
		struct hostent* he = gethostbyname(ip.c_str());
		if(he != 0)
			resolvedIp = *(uint32_t*)he->h_addr;
		else
		{
			std::cout << "ERROR: Cannot resolve " << ip << "!" << std::endl;
			startupErrorMessage("");
		}
	}

	IpNetMask.first  = resolvedIp;
	IpNetMask.second = 0;
	serverIPs.push_back(IpNetMask);

	#if !defined(WIN32) && !defined(__ROOT_PERMISSION__)
	if(getuid() == 0 || geteuid() == 0)
		std::cout << ":: WARNING: " << STATUS_SERVER_NAME << " has been executed as root user, it is recommended to execute is as a normal user." << std::endl;
	#endif

	IOLoginData::getInstance()->resetOnlineStatus();
	g_game.setGameState(GAME_STATE_NORMAL);
	OTSYS_THREAD_SIGNAL_SEND(g_loaderSignal);
}
