1. the old version 5.6.2 support android5
   https://github.com/yuttadhammo/BodhiTimer
2. build env:
    windows10
    java jdk11  jdk17
      https://adoptium.net/temurin/releases/
    Android Command line tools
      https://developer.android.com/studio
	      commandlinetools-win-14742923_latest.zip
3. build tips:
    config java path to jdk17,then
	config sdk:
	  sdkmanager "platform-tools"
	  sdkmanager "platforms;android-30"
	  sdkmanager "build-tools;30.0.1"
	  sdkmanager --licenses
	  
    switch java env to jdk11:
        $env:JAVA_HOME="C:\Program Files\Eclipse Adoptium\jdk-11.0.31.11-hotspot"
		$env:PATH="$env:JAVA_HOME\bin;$env:PATH"
     build:
	   create keystore:
         keytool -genkeypair -v -keystore my-release.jks -keyalg RSA -keysize 2048 -validity 10000 -alias mykey
	   config key.properties from example 
	   then,
        .\gradlew.bat assembleDebug
      build release:
       .\gradlew.bat assembleRelease