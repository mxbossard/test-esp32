#include <WebAnimator.h>

/**
 * constructor
 */
WebAnimator::WebAnimator() {
}

/**
 * destructor
 */
WebAnimator::~WebAnimator() {
    if(_client) {
        _client->end();
    }
}

/**
 * @param client HTTPClient&
 * @return success bool
 */
bool WebAnimator::begin(HTTPClient &client) {
    _client = &client;
}

/**
 * @param client HTTPClient&
 * @return success bool
 */
void WebAnimator::requestAnimation(String url) {
}

/**
 * @param client HTTPClient&
 * @return success bool
 */
void WebAnimator::displayAnimationOnce() {
}