#include <libgui.h>
#include <gccore.h>
#include <lua.hpp>
#include "main.h"
#include "luasupport.h"
#include "stormbreaker.h"

/*
static int lua_PMS2_isLite(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushboolean(L, PMS2::isLite());

    return 1;
}

static int lua_PMS2_getUpdateProgress(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, PMS2::getUpdateProgress());

    return 1;
}

static int lua_PMS2_isUpdating(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushboolean(L, PMS2::isUpdating());

    return 1;
}

static int lua_PMS2_hasUpdateSucceeded(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushboolean(L, PMS2::hasUpdateSucceeded());

    return 1;
}

static int lua_PMS2_startUpdate(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::startUpdate(luaL_checkstring(L, 1));

    return 0;
}
*/

static int lua_STORMBREAKER_isConnected(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushboolean(L, STORMBREAKER::isConnected());

    return 1;
}

static int lua_STORMBREAKER_getVer(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, STORMBREAKER::getVer());

    return 1;
}
/*
static int lua_PMS2_getConf0(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, PMS2::getConf0());

    return 1;
}

static int lua_PMS2_getPot(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, PMS2::getPot());

    return 1;
}
*/

static int lua_STORMBREAKER_getChargeCurrent(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, STORMBREAKER::getChargeCurrent());

    return 1;
}

static int lua_STORMBREAKER_getTermCurrent(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, STORMBREAKER::getTermCurrent());

    return 1;
}

static int lua_STORMBREAKER_getPreChargeCurrent(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, STORMBREAKER::getPreChargeCurrent());

    return 1;
}

static int lua_STORMBREAKER_getChargeVoltage(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, STORMBREAKER::getChargeVoltage());

    return 1;
}

/*
static int lua_PMS2_getTREG(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, PMS2::getTREG());

    return 1;
}
*/

static int lua_STORMBREAKER_getChargeStatus(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, STORMBREAKER::getChargeStatus());

    return 1;
}

/*
static int lua_PMS2_getBatDesignCapacity(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, PMS2::getBatDesignCapacity());

    return 1;
}
*/
static int lua_STORMBREAKER_getSOC(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, STORMBREAKER::getSOC());

    return 1;
}

static int lua_STORMBREAKER_getSOCRaw(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, STORMBREAKER::getSOCRaw());

    return 1;
}


static int lua_STORMBREAKER_getVCell(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, STORMBREAKER::getVCell());

    return 1;
}

static int lua_STORMBREAKER_getVCellRaw(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, STORMBREAKER::getVCellRaw());

    return 1;
}

/*
static int lua_PMS2_getCurrent(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, PMS2::getCurrent());

    return 1;
}

static int lua_PMS2_getCurrentRaw(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, PMS2::getCurrentRaw());

    return 1;
}

static int lua_PMS2_getTTE(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, PMS2::getTTE());

    return 1;
}

static int lua_PMS2_getTTF(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, PMS2::getTTF());

    return 1;
}

static int lua_PMS2_setConf0(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::setConf0(luaL_checkinteger(L, 1));

    return 0;
}
*/

static int lua_STORMBREAKER_setChargeCurrent(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    STORMBREAKER::setChargeCurrent(luaL_checkinteger(L, 1));

    return 0;
}

static int lua_STORMBREAKER_setTermCurrent(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    STORMBREAKER::setTermCurrent(luaL_checkinteger(L, 1));

    return 0;
}

static int lua_STORMBREAKER_setPreChargeCurrent(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    STORMBREAKER::setPreChargeCurrent(luaL_checkinteger(L, 1));

    return 0;
}

static int lua_STORMBREAKER_setChargeVoltage(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    STORMBREAKER::setChargeVoltage(luaL_checkinteger(L, 1));

    return 0;
}

/*
static int lua_PMS2_setTREG(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::setTREG(luaL_checkinteger(L, 1));

    return 0;
}

static int lua_PMS2_setBatDesignCapacity(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::setBatDesignCapacity(luaL_checkinteger(L, 1));

    return 0;
}
*/

static int lua_STORMBREAKER_enableShippingMode(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    STORMBREAKER::enableShippingMode();

    return 0;
}

static int lua_STORMBREAKER_flashConfig(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    STORMBREAKER::flashConfig();

    return 0;
}

/*
static int lua_PMS2_reconfigureMAX(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::reconfigureMAX();

    return 0;
}
*/

static int lua_STORMBREAKER_getNTC(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, STORMBREAKER::getNTC());

    return 1;
}

static int lua_STORMBREAKER_getFanSpeed(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, STORMBREAKER::getFanSpeed());

    return 1;
}

static int lua_STORMBREAKER_setFanSpeed(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    STORMBREAKER::setFanSpeed(luaL_checkinteger(L, 1));

    return 0;
}

/*
static int lua_PMS2_freeFan(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::freeFan();

    return 0;
}

static int lua_PMS2_getFanPIDkP(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, PMS2::getFanPIDkP());

    return 1;
}

static int lua_PMS2_setFanPIDkP(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::setFanPIDkP(luaL_checknumber(L, 1));

    return 0;
}

static int lua_PMS2_getFanPIDkI(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, PMS2::getFanPIDkI());

    return 1;
}

static int lua_PMS2_setFanPIDkI(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::setFanPIDkI(luaL_checknumber(L, 1));

    return 0;
}

static int lua_PMS2_getFanPIDkD(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, PMS2::getFanPIDkD());

    return 1;
}

static int lua_PMS2_setFanPIDkD(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::setFanPIDkD(luaL_checknumber(L, 1));

    return 0;
}

static int lua_PMS2_getFanPIDTarget(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, PMS2::getFanPIDTarget());

    return 1;
}

static int lua_PMS2_setFanPIDTarget(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::setFanPIDTarget(luaL_checknumber(L, 1));

    return 0;
}

static int lua_PMS2_getFanRange(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::fanRange_t range = PMS2::getFanRange();

    lua_newtable(L);
    luaSetTableIntField(L, "min", range.min);
    luaSetTableIntField(L, "max", range.max);

    return 1;
}

static int lua_PMS2_setFanRange(lua_State* L) {
    PMS2::fanRange_t range;
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    luaL_checktype(L, 1, LUA_TTABLE);
    //Grab fields
    lua_getfield(L, 1, "min");
    lua_getfield(L, 1, "max");

    range.min = luaL_checkinteger(L, -2);
    range.max = luaL_checkinteger(L, -1);

    lua_pop(L, 3); //Pop table and range data

    PMS2::setFanRange(range);

    return 0;
}

static int lua_PMS2_getLEDIntensity(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushinteger(L, PMS2::getLEDIntensity());

    return 1;
}

static int lua_PMS2_setLEDIntensity(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    PMS2::setLEDIntensity(luaL_checkinteger(L, 1));

    return 0;
}
*/

static const luaL_Reg STORMBREAKER_functions[] = {
    //{"isLite", lua_PMS2_isLite},
    //{"getUpdateProgress", lua_PMS2_getUpdateProgress},
    //{"isUpdating", lua_PMS2_isUpdating},
    //{"startUpdate", lua_PMS2_startUpdate},
    {"isConnected", lua_STORMBREAKER_isConnected},
    //{"hasUpdateSucceeded", lua_PMS2_hasUpdateSucceeded},
    {"getVer", lua_STORMBREAKER_getVer},
    //{"getConf0", lua_PMS2_getConf0},
    //{"getPot", lua_PMS2_getPot},
    {"getChargeCurrent", lua_STORMBREAKER_getChargeCurrent},
    {"getTermCurrent", lua_STORMBREAKER_getTermCurrent},
    {"getPreChargeCurrent", lua_STORMBREAKER_getPreChargeCurrent},
    {"getChargeVoltage", lua_STORMBREAKER_getChargeVoltage},
    //{"getTREG", lua_PMS2_getTREG},
    {"getChargeStatus", lua_STORMBREAKER_getChargeStatus},
    //{"getBatDesignCapacity", lua_PMS2_getBatDesignCapacity},
    //{"getSOC", lua_PMS2_getSOC},
    //{"getSOCRaw", lua_PMS2_getSOCRaw},
    {"getVCell", lua_STORMBREAKER_getVCell},
    {"getVCellRaw", lua_STORMBREAKER_getVCellRaw},
    //{"getCurrent", lua_PMS2_getCurrent},
    //{"getCurrentRaw", lua_PMS2_getCurrentRaw},
    //{"getTTE", lua_PMS2_getTTE},
    //{"getTTF", lua_PMS2_getTTF},
    //{"setConf0", lua_PMS2_setConf0},
    {"setChargeCurrent", lua_STORMBREAKER_setChargeCurrent},
    {"setTermCurrent", lua_STORMBREAKER_setTermCurrent},
    {"setPreChargeCurrent", lua_STORMBREAKER_setPreChargeCurrent},
    {"setChargeVoltage", lua_STORMBREAKER_setChargeVoltage},
    //{"setTREG", lua_PMS2_setTREG},
    //{"setBatDesignCapacity", lua_PMS2_setBatDesignCapacity},
    {"enableShippingMode", lua_STORMBREAKER_enableShippingMode},
    {"flashConfig", lua_STORMBREAKER_flashConfig},
    //{"reconfigureMAX", lua_PMS2_reconfigureMAX},
    {"getNTC", lua_STORMBREAKER_getNTC},
    {"getFanSpeed", lua_STORMBREAKER_getFanSpeed},
    {"setFanSpeed", lua_STORMBREAKER_setFanSpeed},
    //{"freeFan", lua_PMS2_freeFan},
    //{"getFanPIDkP", lua_PMS2_getFanPIDkP},
    //{"setFanPIDkP", lua_PMS2_setFanPIDkP},
    //{"getFanPIDkI", lua_PMS2_getFanPIDkI},
    //{"setFanPIDkI", lua_PMS2_setFanPIDkI},
    //{"getFanPIDkD", lua_PMS2_getFanPIDkD},
    //{"setFanPIDkD", lua_PMS2_setFanPIDkD},
    //{"getFanPIDTarget", lua_PMS2_getFanPIDTarget},
    //{"setFanPIDTarget", lua_PMS2_setFanPIDTarget},
    //{"getFanRange", lua_PMS2_getFanRange},
    //{"setFanRange", lua_PMS2_setFanRange},
    //{"getLEDIntensity", lua_PMS2_getLEDIntensity},
    //{"setLEDIntensity", lua_PMS2_setLEDIntensity},
    {NULL, NULL}
};

void luaRegisterSTORMBREAKERLib(lua_State* L) {
    lua_newtable(L);
    luaL_setfuncs(L, STORMBREAKER_functions, 0);
    luaSetTableIntField(L, "PWR_BTN_MOMEN", 0x00);
    luaSetTableIntField(L, "PWR_BTN_TOGGLE", 0x01);
    luaSetTableIntField(L, "PWR_BTN_TYPE", 0x01);
    luaSetTableIntField(L, "PWR_BTN_ACTLOW", 0x00);
    luaSetTableIntField(L, "PWR_BTN_ACTHI", 0x02);
    luaSetTableIntField(L, "PWR_BTN_POLARITY", 0x02);
    luaSetTableIntField(L, "STAT_LED_STD", 0x00);
    luaSetTableIntField(L, "STAT_LED_WSD", 0x04);
    luaSetTableIntField(L, "STAT_LED_WSB", 0x08);
    luaSetTableIntField(L, "STAT_LED_TYPE", 0x0C);
    luaSetTableIntField(L, "CHG_STAT_NOT_CHG", CHG_STAT_NOT_CHG);
    luaSetTableIntField(L, "CHG_STAT_PRE_CHG", CHG_STAT_PRE_CHG);
    luaSetTableIntField(L, "CHG_STAT_FAST_CHG", CHG_STAT_FAST_CHG);
    luaSetTableIntField(L, "CHG_STAT_DONE", CHG_STAT_DONE);
    lua_setglobal(L, "STORMBREAKER");
}
