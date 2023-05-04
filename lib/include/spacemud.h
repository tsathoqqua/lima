#ifndef __SPACEMUD
#define __SPACEMUD

//Classes
#define SPACE_CLASSES   "/std/classes/space"

//Daemons
#define SPACE_D         "/daemons/space_d"
#define SPACESTATION_D  "/daemons/spacestation_d"
#define SHIP_D          "/daemons/ship_d"
#define GOVERNANCE_D    "/daemons/governance_d"

//Items
#define SPACESHIP       "/std/spaceship"
#define SHIP_PATH(x)	sprintf("/data/ships/%c/%s",x[0],replace_string(base_name()[1..],"/","_"))
#define VDU_MENU        "/obj/mudlib/vdu_menu"
#define DOCKING_MENU    "/obj/mudlib/docking_menu"
#define SHIP_NAV_MENU   "/obj/mudlib/ship_nav_menu"

//Modules
#define M_GUILD_MASTER  "/std/modules/m_guild_master"
#define M_GUILD_MEMBER  "/std/modules/m_guild_member"

#include "/domains/common/include/common.h"

#endif
