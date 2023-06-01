#include <FastBot.h>

#define BOT_TOKEN "5154514963:AAEUHp0J9WNkl_G1adHtDjWZ0E1jGf9xbKU"
#define CHAT_ID "5374956150"

FastBot bot(BOT_TOKEN);

// Use @myidbot (IDBot) to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you

void newMsg(FB_msg &msg)
{
    // выводим имя юзера и текст сообщения
    // Serial.print(msg.username);
    // Serial.print(", ");
    // Serial.println(msg.text);

    // выводим всю информацию о сообщении
    Serial.println(msg.toString());
}