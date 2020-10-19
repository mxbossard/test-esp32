#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include <U8g2lib.h>

class WebAnimator {

public:
    WebAnimator();
    ~WebAnimator();

    bool begin(HTTPClient &client, U8G2 &display);

    void requestAnimation(String url);

    void displayAnimationOnce();

    void displayText(String text);

protected:
    HTTPClient* _client = nullptr;
    U8G2* _display = nullptr;
    DynamicJsonDocument* _doc = nullptr;
    int _screenHeight = 128;
    int _screenWidth = 64;
    int _fontHeight = 14;
    int _fontWidth = 7;

    void displayPageInternal(JsonObject &page);

    void displayPagesInternal(DynamicJsonDocument &doc);
};