#!/bin/bash
gradlew :app:installDebug
adb shell am start -n "me.shiki.nativeopengllearn/me.shiki.nativeopengllearn.MainActivity" -a android.intent.action.MAIN -c android.intent.category.LAUNCHER