#include <HTTPClient.h>

class WebAnimator {

public:
    WebAnimator();
    ~WebAnimator();

    bool begin(HTTPClient &client);

    void requestAnimation(String url);

    void displayAnimationOnce();

protected:
    HTTPClient* _client = nullptr;

};