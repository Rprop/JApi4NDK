/*
 *
 *	@author : rrrfff@foxmail.com
 *  https://github.com/rrrfff/JApi4NDK
 *
 */
#pragma once
#include <jni.h>
#ifndef	__selectany
# define __selectany __attribute__((weak))
#endif // __selectany

//-------------------------------------------------------------------------

class DexClassLoader
{
	// dalvik/system/DexClassLoader
	static jclass    clsDexClassLoader_;
	static jmethodID loadClass_;
	static jmethodID constructor_;
	// android/content/Context
	static jmethodID getClassLoader_;
	// java/lang/ClassLoader
	static jclass    clsClassLoader_;
	static jmethodID getSysClassLoader_;

protected:
	jobject dexloader_;

public:
	DexClassLoader(JNIEnv *env, const char *dexPath, const char *optDir,
				   const char *libPath, jobject sysLoader) {
		env->PushLocalFrame(4);
		jstring jdexPath = env->NewStringUTF(dexPath);
		jstring joptDir = env->NewStringUTF(optDir);
		jstring jlibPath = env->NewStringUTF(libPath);
		jobject dexloader = env->NewObject(clsDexClassLoader_, constructor_, jdexPath, joptDir,
										   jlibPath, sysLoader);
		if (dexloader == NULL) {
			env->ExceptionDescribe(); env->ExceptionClear();
			this->dexloader_ = NULL;
		} else {
			this->dexloader_ = env->NewGlobalRef(dexloader);
		} //if
		env->PopLocalFrame(NULL);
	}

public:
	static void init(JNIEnv *env) {
		env->PushLocalFrame(3);
		clsDexClassLoader_ = reinterpret_cast<jclass>(env->NewWeakGlobalRef(env->FindClass("dalvik/system/DexClassLoader")));
		constructor_ = env->GetMethodID(clsDexClassLoader_, "<init>",
										"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
		loadClass_ = env->GetMethodID(clsDexClassLoader_, "loadClass",
									  "(Ljava/lang/String;)Ljava/lang/Class;");

		getClassLoader_ = env->GetMethodID(env->FindClass("android/content/Context"),
										   "getClassLoader", "()Ljava/lang/ClassLoader;");

		clsClassLoader_ = reinterpret_cast<jclass>(env->NewWeakGlobalRef(env->FindClass("java/lang/ClassLoader")));
		getSysClassLoader_ = env->GetStaticMethodID(clsClassLoader_,
												 "getSystemClassLoader", "()Ljava/lang/ClassLoader;");

		env->PopLocalFrame(NULL);
	}
	static jobject getClassLoader(JNIEnv *env, jobject context) {
		return env->CallObjectMethod(context, getClassLoader_);
	}
	static jobject getSystemLoader(JNIEnv *env) {
		return env->CallStaticObjectMethod(clsClassLoader_, getSysClassLoader_);
	}

public:
	jobject getLoader() {
		return this->dexloader_;
	}
	jclass loadClass(JNIEnv *env, const char *className) {
		if (this->dexloader_ == NULL) return NULL;

		jstring jcn = env->NewStringUTF(className);
		jobject cls = env->CallObjectMethod(this->dexloader_, this->loadClass_, jcn);
		env->DeleteLocalRef(jcn);
		if (env->ExceptionCheck()) {
			env->ExceptionDescribe(); env->ExceptionClear();
		} //if
		return reinterpret_cast<jclass>(cls);
	}
};
// dalvik/system/DexClassLoader
__selectany jclass    DexClassLoader::clsDexClassLoader_;
__selectany jmethodID DexClassLoader::loadClass_;
__selectany jmethodID DexClassLoader::constructor_;
// android/content/Context
__selectany jmethodID DexClassLoader::getClassLoader_;
// java/lang/ClassLoader
__selectany jclass    DexClassLoader::clsClassLoader_;
__selectany jmethodID DexClassLoader::getSysClassLoader_;
