// Contents of DLLDefines.h
#ifndef __DLL_DEFINES_H_
#define __DLL_DEFINES_H_

#if defined (_WIN32) 
	#if defined(moldinam_EXPORTS)
	    #define  MOLDINAM_EXPORT __declspec(dllexport)
	#else
		#define  MOLDINAM_EXPORT __declspec(dllimport)
	#endif /* moldinam_EXPORTS */
#else /* defined (_WIN32) */
	#define MOLDINAM_EXPORT
#endif

#endif /* __DLL_DEFINES_H_ */