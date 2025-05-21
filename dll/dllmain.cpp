#include "../shared.h"
#include <iostream>
#include <jni.h>
#include <jvmti.h>
#include <vector>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include "minecraft.h"

HMODULE module;

DWORD WINAPI THREAD(LPVOID _) {
    #ifdef VERBOSE
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    AttachConsole(GetCurrentProcessId());
    SetConsoleTitleA("Debug");
    #endif

    JavaVM* jvm = new JavaVM[1];
    
    JNI_GetCreatedJavaVMs(&jvm, 1, nullptr);
    
    JNIEnv* env = nullptr;

    if (jvm->GetEnv((void**)&env, JNI_VERSION_1_8) == JNI_EDETACHED) {
        jvm->AttachCurrentThreadAsDaemon((void**)&env, NULL);
    }

    jvmtiEnv* jvmti = nullptr;

    jvm->GetEnv((void**)&jvmti, JVMTI_VERSION_1_2);

    minecraft mc = minecraft(env, jvmti);

    jclass client = mc.get_class("net.minecraft.client.Minecraft");
    jclass entityPlayerSP = mc.get_class("net.minecraft.client.entity.EntityPlayerSP");

    jfieldID theMinecraftField = env->GetStaticFieldID(client, "theMinecraft", "Lnet/minecraft/client/Minecraft;");
    jfieldID thePlayerField = env->GetFieldID(client, "thePlayer", "Lnet/minecraft/client/entity/EntityPlayerSP;");

    jobject theMinecraft = env->GetStaticObjectField(client, theMinecraftField);
    jobject thePlayer = env->GetObjectField(theMinecraft, thePlayerField);

    jmethodID sendChatMessage = env->GetMethodID(entityPlayerSP, "sendChatMessage", "(Ljava/lang/String;)V");

    env->CallVoidMethod(thePlayer, sendChatMessage, env->NewStringUTF("balls"));

    std::cout << thePlayer << std::endl;
   
    FreeLibraryAndExitThread(module, 0);
    return FALSE;
}

static BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            module = hModule;
            //CreateThread(NULL, 0, THREAD, 0, 0, NULL);
            //DisableThreadLibraryCalls(module);
            printf("Hello");
    }
    return TRUE;
}
