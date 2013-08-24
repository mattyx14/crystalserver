//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
//
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

#ifndef __OTSERV_TOOLS_H__
#define __OTSERV_TOOLS_H__

#include "otsystem.h"
#include "position.h"
#include "const.h"
#include "enums.h"

#include <string>
#include <algorithm>

#include <libxml/parser.h>

#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

enum DistributionType_t
{
	DISTRO_UNIFORM,
	DISTRO_SQUARE,
	DISTRO_NORMAL
};

extern std::string transformToMD5(std::string plainText, bool upperCase = false);
extern std::string transformToSHA1(std::string plainText, bool upperCase = false);
extern bool passwordTest(const std::string &plain, std::string &hash);

extern void replaceString(std::string& str, const std::string sought, const std::string replacement);
extern void trim_right(std::string& source, const std::string& t);
extern void trim_left(std::string& source, const std::string& t);
extern void toLowerCaseString(std::string& source);
extern void toUpperCaseString(std::string& source);
extern std::string asLowerCaseString(const std::string& source);
extern std::string asUpperCaseString(const std::string& source);

extern bool utf8ToLatin1(char* intext, std::string& outtext);
extern bool readXMLInteger(xmlNodePtr node, const char* tag, int& value);
#if (defined __WINDOWS__ || defined WIN32) && !defined __GNUC__
extern bool readXMLInteger(xmlNodePtr node, const char* tag, int32_t& value);
#endif
extern bool readXMLInteger64(xmlNodePtr node, const char* tag, uint64_t& value);
extern bool readXMLFloat(xmlNodePtr node, const char* tag, float& value);
extern bool readXMLString(xmlNodePtr node, const char* tag, std::string& value);
extern bool readXMLContentString(xmlNodePtr node, std::string& value);

extern std::vector<std::string> explodeString(const std::string& inString, const std::string& separator);
extern std::vector<int32_t> vectorAtoi(std::vector<std::string> stringVector);
extern bool hasBitSet(uint32_t flag, uint32_t flags);

extern std::string generateRecoveryKey(int32_t fieldCount, int32_t fieldLength);

extern bool isNumber(char character);
extern bool isLowercaseLetter(char character);
extern bool isPasswordCharacter(char character);

extern bool isValidName(std::string text);
extern bool isValidPassword(std::string text);
extern bool isNumbers(std::string text);

extern bool checkText(std::string text, std::string str);

extern int32_t random_range(int lowest_number, int highest_number, DistributionType_t type = DISTRO_UNIFORM);

extern Direction getDirection(std::string string);
extern Direction getReverseDirection(Direction dir);
extern Position getNextPosition(Direction direction, Position pos);

extern char upchar(char c);

extern std::string parseParams(tokenizer::iterator &it, tokenizer::iterator end);

extern void formatDate(time_t time, char* buffer);
extern void formatDate2(time_t time, char* buffer);
extern void formatIP(uint32_t ip, char* buffer);
extern std::string formatTime(int32_t hours, int32_t minutes);

extern std::string trimString(std::string& str);

extern MagicEffectClasses getMagicEffect(const std::string& strValue);
extern ShootType_t getShootType(const std::string& strValue);
extern Ammo_t getAmmoType(const std::string& strValue);
extern AmmoAction_t getAmmoAction(const std::string& strValue);

extern std::string getSkillName(uint16_t skillid);
extern skills_t getSkillId(std::string param);

extern std::string getReason(int32_t reasonId);
extern std::string getAction(int32_t actionId, bool IPBanishment);

extern bool fileExists(const char* filename);
extern bool booleanString(const std::string& str);
#endif
