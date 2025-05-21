#include <iostream>
#include <jni.h>
#include <jvmti.h>
#include <unordered_map>
#include <string>

class minecraft
{
public:
	minecraft(JNIEnv* env, jvmtiEnv* jvmti);

	jclass get_class(std::string name);

private:

	JNIEnv* env;
	jvmtiEnv* jvmti;
	std::unordered_map<std::string, jclass> class_map;
};