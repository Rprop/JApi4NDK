/*
 *
 *	@author : rrrfff@foxmail.com
 *  https://github.com/rrrfff/JApi4NDK
 *
 */
#pragma once
#include <jni.h>

//-------------------------------------------------------------------------

class ActivityThread
{
public:
	static jclass getClass(JNIEnv *env) {
// 		static jclass clazz = NULL;
// 		if (clazz == NULL) {
// 			// possible race conditions
// 			clazz = env->NewWeakGlobalRef(env->FindClass("android/app/ActivityThread"));
// 		} //if
		return env->FindClass("android/app/ActivityThread");
	}
	static jobject getSystemContext(JNIEnv *env) {
		jobject th   = ActivityThread::currentActivityThread(env);
		jmethodID md = env->GetMethodID(ActivityThread::getClass(env), 
										"getSystemContext", "()Landroid/app/ContextImpl;");
		return env->CallObjectMethod(th, md);
	}
	static jobject currentActivityThread(JNIEnv *env) {
		jclass clazz = ActivityThread::getClass(env);
		jmethodID md = env->GetStaticMethodID(clazz, "currentActivityThread", "()Landroid/app/ActivityThread;");
		return env->CallStaticObjectMethod(clazz, md);
	}
	static jobject currentApplication(JNIEnv *env) {
		jclass clazz = ActivityThread::getClass(env);
		jmethodID md = env->GetStaticMethodID(clazz, "currentApplication", "()Landroid/app/Application;");
		return env->CallStaticObjectMethod(clazz, md);
	}
};