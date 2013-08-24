	-- Crystal Server Config

	-- SQL Connection
	sqlType = "sqlite"
	passwordType = "plain"
	startupDatabaseOptimization = "yes"
	mysqlHost = "localhost"
	mysqlUser = "root"
	mysqlPass = ""
	mysqlPort = 3306
	mysqlDatabase = "crystalserver"
	sqliteDatabase = "crystalserver.s3db"

	-- Account Manager
	accountManager = "yes"
	newPlayerChooseVoc = "no"
	newPlayerSpawnPosX = 95
	newPlayerSpawnPosY = 117
	newPlayerSpawnPosZ = 7
	newPlayerTownId = 1
	newPlayerLevel = 1
	newPlayerMagicLevel = 0
	generateAccountNumber = "no"

	-- Banishments
	useRuleViolationReport = "yes"
	broadcastBanishments = "yes"
	killsToBan = 5

	-- Battle
	worldType = "pvp"
	hotkeyAimbotEnabled = "yes"
	protectionLevel = 1
	killsToRedSkull = 3
	pzLocked = 60000
	deathLosePercent = 10
	criticalHitChance = 7
	removeAmmoWhenUsingDistanceWeapon = "yes"
	removeChargesFromRunes = "yes"
	timeToDecreaseFrags = 24 * 60 * 60 * 1000
	whiteSkullTime = 15 * 60 * 1000

	-- Commands
	displayGamemastersWithOnlineCommand = "no"

	-- Connection Config
	ip = "127.0.0.1"
	port = 7171
	loginTries = 10
	retryTimeout = 5 * 1000
	loginTimeout = 60 * 1000
	maxPlayers = "1000"
	motd = "Welcome to the Crystal Server!"
	onePlayerOnlinePerAccount = "yes"
	allowClones = 0
	serverName = "Crystal"
	loginMessage = "Welcome to the Crystal Server!"
	adminLogsEnabled = "no"
	statusTimeout = 5 * 60 * 1000
	replaceKickOnLogin = "yes"

	-- Deathlist
	deathListEnabled = "yes"
	maxDeathRecords = 5

	-- Guilds
	ingameGuildSystem = "yes"

	-- Highscores
	highscoreDisplayPlayers = 15
	updateHighscoresAfterMinutes = 60

	-- Houses
	housePriceEachSQM = 1000
	houseRentPeriod = "never"

	-- Idle
	kickIdlePlayerAfterMinutes = 15

	-- Item Usage
	timeBetweenActions = 200
	timeBetweenExActions = 1000

	-- Map
	mapName = "forgotten"
	mapAuthor = ""
	randomizeTiles = "yes"

	-- Messaging
	maxMessageBuffer = 4

	-- Premium Account
	freePremium = "no"

	-- PVP Server
	displayOnOrOffAtCharlist = "no"
	allowChangeOutfit = "yes"
	noDamageToSameLookfeet = "no"
	experienceByKillingPlayers = "no"

	-- Rates
	rateExperience = 1
	rateSkill = 1
	rateLoot = 1
	rateMagic = 1
	rateSpawn = 1

	-- Stamina
	useStaminaSystem = "yes"

	-- Real Server Save
	-- note: serverSaveHour means like 03:00, not that it will save every 3 hours,
	-- if you want such a system use autoSaveEachMinutes. this serversave method
	-- may be unstable, we recommend using otadmin if you want real serversaves.
	serverSaveEnabled = "no"
	serverSaveHour = 3
	shutdownAtServerSave = "yes"
	cleanMapAtServerSave = "yes"

	-- Server saving
	autoSaveEachMinutes = 15
	saveGlobalStorage = "no"

	-- Spawns
	deSpawnRange = 2
	deSpawnRadius = 50

	-- Startup
	defaultPriority = "high"

	-- Status
	ownerName = ""
	ownerEmail = ""
	url = "https://github.com/tryller/crystalserver"
	location = "Brazil"