#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_me_shiki_nativeopengllearn_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from demo_android";
    return env->NewStringUTF(hello.c_str());
}