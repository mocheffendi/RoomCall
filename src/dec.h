char buff[18];

void reverse(char *x, int begin, int end)
{
    char c;
    if (begin >= end)
        return;
    c = *(x + begin);
    *(x + begin) = *(x + end);
    *(x + end) = c;
    reverse(x, ++begin, --end);
}
String decimal(String n)
{
    // bot.sendMessage("String n : " + n);
    String ans;
    int count = 0;
    for (int i = n.length() - 1; i >= 0; i--)
    {
        ans += n[i];
        count++;
        if (count == 3)
        {
            ans += ('.');
            count = 0;
        }
    }
    if (ans.length() % 4 == 0)
    {
        ans.remove(ans.length() - 1, 1);
    }
    ans.toCharArray(buff, 18);
    reverse(buff, 0, ans.length() - 1);

    // bot.sendMessage("Buff : " + String(buff));
    return String(buff);
}

int bytestoKB(int bkb)
{
    return bkb = bkb / 1024;
}