CFLAGS = -I. -I/usr/include/libxml2 -I/usr/include/lua5.1

LIBLINK = -L/usr/lib -lxml2 -lpthread -llua5.1 -lboost_thread -lgmp -lmysqlclient -lboost_regex -lsqlite3 -llua5.1-sql-mysql -ldl -lboost_system

FLAGS = -D__USE_MYSQL__ -D__USE_SQLITE__

OBJ = account.o actions.o allocator.o ban.o baseevents.o beds.o creature.o creatureevent.o chat.o combat.o commands.o condition.o configmanager.o connection.o container.o cylinder.o database.o databasemysql.o databasesqlite.o depot.o exception.o fileloader.o game.o house.o housetile.o ioguild.o iologindata.o iomap.o iomapserialize.o item.o items.o logger.o luascript.o mailbox.o map.o md5.o monster.o monsters.o movement.o networkmessage.o npc.o otserv.o outfit.o outputmessage.o party.o player.o position.o protocol.o protocolgame.o protocollogin.o quests.o raids.o scheduler.o scriptmanager.o server.o sha1.o spawn.o spells.o status.o talkaction.o tasks.o teleport.o thing.o tile.o tools.o trashholder.o vocation.o waitlist.o weapons.o 

all: clean crystalserver

clean:
	rm -rf *.o

crystalserver: $(OBJ)
	g++ $(CFLAGS) $(FLAGS) -Werror -O2 -o ./CrystalServer $(OBJ) $(LIBLINK)

    %.o:%.cpp
	g++ $(CFLAGS) $(FLAGS) -Werror -O2 -c $+
