# XCode configuration
## add -fobjc-arc for IAPHelper and IAPShare
## add SystemConfigurate.framework
## change CSC_IOSHelper.cpp to CSC_IOSHelper.mm

# use git subtree command to add this in the project
## git remote add -f CSCClass https://github.com/reallycsc/CSCClass.git
<<<<<<< HEAD
## git subtree add --prefix=Classes/CSCClass CSCClass master
## git fetch CSCClass master
## git subtree pull --prefix=Classes/CSCClass CSCClass master
=======
## git subtree add --prefix=Classes/CSCClass CSCClass master --squash
## git fetch CSCClass master
## git subtree pull --prefix=Classes/CSCClass CSCClass --squash
>>>>>>> f4ba7c47cb80ca9fbcd0a85aba3e39e01e83d01c
## git subtree push --prefix=Classes/CSCClass CSCClass master