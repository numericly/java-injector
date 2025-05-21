#include <iostream>
#include <jni.h>
#include <jvmti.h>
#include <unordered_map>
#include <string>
#include "minecraft.h";


minecraft::minecraft(JNIEnv* env, jvmtiEnv* jvmti): env(env), jvmti(jvmti) {
	jint count;
	jclass* classes;

	this->jvmti->GetLoadedClasses(&count, &classes);

	jclass langClass = this->env->FindClass("java/lang/Class");
	jmethodID getName = this->env->GetMethodID(langClass, "getName", "()Ljava/lang/String;");

	for (int i = 0; i < static_cast<int>(count); i++) {
		jclass clazz = classes[i];
		jstring name = (jstring)this->env->CallObjectMethod(clazz, getName);
		const char* class_name = this->env->GetStringUTFChars(name, false);

		this->class_map.insert({ class_name, clazz });
	}

	printf("loaded %s classes\n", std::to_string(this->class_map.size()).c_str());


}


jclass minecraft::get_class(std::string name) {
	return this->class_map[name];
}