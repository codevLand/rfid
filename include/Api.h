#ifndef API_H
#define API_H

typedef struct {
  String name;
  uint8_t port;
  String env;
  String level;
} API_SERVER;

class Api {
  private:
    /* data */

  public:
    Api(API_SERVER api) {}
    ~Api() {}

    void routes();

  protected:
    /* dont ask me why */
};

#endif
