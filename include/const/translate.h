#pragma once

enum TRANSLATE_LANGUAGE{
    TRANSLATE_LANG_EN = 0,
    TRANSLATE_LANG_RU,
    TRANSLATE_LANG_COUNT
};

enum class Dictionary{
    MENU = 0,
    HELP,
    CHOOSE_LANG,
    DONE,
    WRONG_VALUE,
    BUTTON_PRESSED_SHORT,
    BUTTON_PRESSED_LONG,
    SETTINGS,
    REQUEST_SHORT_PRESS_TIME_FMT,
    REQUEST_LONG_PRESS_TIME_FMT,
    USER_MANAGEMENT,
    GIVE_ACCESS,
    USER_NEVER_REQUESTED_ACCESS,
    REVOKE_ACCESS,
    USER_DOESNT_HAVE_ACCESS_TO_THIS_APP,
    HELP_INFO,
    TOO_MANY_USERS_TRYING_TO_GET_ACCESS,
    ACCESS_REQUESTED,
    ACCESS_REQUESTED_MESSAGE_ADMIN,
    HELP_INFO_UNREGISTERED,

    DICTIONARY_COUNT
};

const char* const en_strings[] PROGMEM = {
    "Menu", //MENU
    "Help", //HELP
    "Choose language", //CHOOSE_LANG
    "Done", //DONE
    "Wrong value", //WRONG_VALUE
    "Button pressed short", //BUTTON_PRESSED_SHORT
    "Button pressed long", //BUTTON_PRESSED_LONG
    "Settings", //SETTINGS
    "Send value from %d to %d (in ms)",//REQUEST_SHORT_PRESS_TIME_FMT
    "Send value from %d to %d (in ms)",//REQUEST_LONG_PRESS_TIME_FMT
    "User management",//USER_MANAGEMENT
    "Give access to a user by their code.\nEnter user's code or select from menu",//GIVE_ACCESS
    "This user never requested the access",//USER_NEVER_REQUESTED_ACCESS
    "Revoke user's permission to use this app.\nEnter user's code or select from menu",//REVOKE_ACCESS
    "This user doesn't have access to this application",//USER_DOESNT_HAVE_ACCESS_TO_THIS_APP
    "Welcome to the Turn On My PC bot!\nThe command list is:\n\n/start - show menu;\n/help - show this message",//HELP_INFO
    "Too many users trying to get access. Ask administrator to clear waiting list",//TOO_MANY_USERS_TRYING_TO_GET_ACCESS
    "Access requested. Wait for admin to give you an access. Tell them this code: %s",//ACCESS_REQUESTED
    "Some user required access to this app. They should tell you this this code: %s.\nIn case you want to authorize this user, use this command %s",//ACCESS_REQUESTED_MESSAGE_ADMIN
    "Welcome to the Turn On My PC bot!\nCurrently you have no access to use any of commands but these:\n\n/start - show menu;\n/help - show this message",//HELP_INFO_UNREGISTERED
};

const char* const ru_strings[] PROGMEM = {
    "Меню", //MENU
    "Справка", //HELP
    "Выберите язык", //CHOOSE_LANG
    "Готово", //DONE
    "Ошибка в значении", //WRONG_VALUE
    "Кнопка нажата кратковременно", //BUTTON_PRESSED_SHORT
    "Кнопка нажата длительно", //BUTTON_PRESSED_LONG
    "Параметры", //SETTINGS
    "Отправьте значение от %d до %d (в мс)",//REQUEST_SHORT_PRESS_TIME_FMT
    "Отправьте значение от %d до %d (в мс)",//REQUEST_LONG_PRESS_TIME_FMT
    "Управление пользователями",//USER_MANAGEMENT
    "Разрешите пользователю пользоваться приложением по его коду доступа.\nВведите код доступа пользователя, или выберите из меню",//GIVE_ACCESS
    "Данный пользователь не запрашивал доступ к приложению",//USER_NEVER_REQUESTED_ACCESS
    "Отзовите разрешение пользоваться приложением у пользователя.\nВведите код доступа пользователя, или выберите из меню",//REVOKE_ACCESS
    "У этого пользователя нет доступа к приложению",//USER_DOESNT_HAVE_ACCESS_TO_THIS_APP
    "Добро пожаловать в бот Turn On My PC!\nСписок команд следующий:\n\n/start - показать меню;\n/help - показать это сообщение",//HELP_INFO
    "Слишком много пользователей пытается получить доступ. Попросите администратора очистить список ожидания",//TOO_MANY_USERS_TRYING_TO_GET_ACCESS
    "Доступ запрошен. Ожидайте пока администратор выдаст вам доступ. Назовите ему этот код: %s",//ACCESS_REQUESTED
    "Кто-то запросил доступ в приложение. Они скажут вам этот код: %s.\nВ случае, если вы хотите разрешить этому пользователю доступ в приложение, воспользуйтесь этой командой: %s",//ACCESS_REQUESTED_MESSAGE_ADMIN
    "Добро пожаловать в бот Turn On My PC!\nВ данный момент, у вас нет доступа к приложению и вы можете только использовать эти команды:\n\n/start - показать меню;\n/help - показать это сообщение",//HELP_INFO_UNREGISTERED
};

const char* const* const strings[TRANSLATE_LANG_COUNT] PROGMEM = {
    en_strings,
    ru_strings
};

inline const char* translate_getMessage(TRANSLATE_LANGUAGE lang, Dictionary message) {
    if (lang < 0 || lang >= TRANSLATE_LANG_COUNT || (int)message < 0 || (int)message >= (int)Dictionary::DICTIONARY_COUNT) {
        return "ERROR_T0";
    }

    const char* const* langStrings = (const char* const*)pgm_read_ptr(&strings[lang]);
    const char* strPtr = (const char*)pgm_read_ptr(&langStrings[int(message)]);
    return strPtr;
}