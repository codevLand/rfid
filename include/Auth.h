#ifndef AUTH_H
#define AUTH_H

typedef struct {
  Uid cardID;
  string firstname;
  string middlename;
  string lastname;
  string room;
  string level;
} User;

class Auth {
  private:
    User user

  public:
    Auth(/* args */) {}
    ~Auth() {}

    // administration
    void signin(User user);
    void signout(User user);
    bool signup(User user);
    bool grantAccess(User user);
    bool denyAccess(User user)

    // gate access
    void verifyUser();
    void allowGatePassage(User user);
    void denyGatePassage();
    void showAllowedUsers();

};