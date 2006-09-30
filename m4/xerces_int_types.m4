dnl @synopsis XERCES_INT_TYPES
dnl
dnl Determines what int types to use for various
dnl Xerces standard integer types.
dnl
dnl @category C
dnl @author James Berry
dnl @version 2005-06-07
dnl @license AllPermissive
dnl
dnl $Id$

AC_DEFUN([XERCES_INT_TYPES],
	[
	AC_CHECK_HEADERS([inttypes.h])
	AC_CHECK_SIZEOF(short)
	AC_CHECK_SIZEOF(int)
	AC_CHECK_SIZEOF(long)
	AC_CHECK_TYPE(int16_t)
	AC_CHECK_TYPE(int32_t)
	AC_CHECK_TYPE(uint16_t)
	AC_CHECK_TYPE(uint32_t)
	
	#
	# Select a signed 16 bit integer type
	#
	AC_CACHE_CHECK([for an appropriate signed 16 bit integer type], [xerces_cv_type_s16bit_int], [
		AS_IF([test x$ac_cv_header_inttypes_h = xyes && test x$ac_cv_type_int16_t = xyes],
			[xerces_cv_type_s16bit_int=int16_t],
			[
				case $ac_cv_sizeof_int in
				2*) xerces_cv_type_s16bit_int=int;;
				*)  
					case $ac_cv_sizeof_short in
					2*) xerces_cv_type_s16bit_int=short;;
					*)  AC_MSG_ERROR([Couldn't find a signed 16 bit int type]);;
					esac
					;;
				esac
			])
	])
	
	#
	# Select an unsigned 16 bit integer type
	#
	AC_CACHE_CHECK([for an appropriate unsigned 16 bit integer type], [xerces_cv_type_u16bit_int], [
		AS_IF([test x$ac_cv_header_inttypes_h = xyes && test x$ac_cv_type_uint16_t = xyes],
			[xerces_cv_type_u16bit_int=uint16_t],
			[
				case $ac_cv_sizeof_int in
				2*) xerces_cv_type_u16bit_int="unsigned int";;
				*)  
					case $ac_cv_sizeof_short in
					2*) xerces_cv_type_u16bit_int="unsigned short";;
					*)  AC_MSG_ERROR([Couldn't find an unsigned 16 bit int type]);;
					esac
					;;
				esac
			])
	])
	
	
	#
	# Select a signed 32 bit integer type
	#
	AC_CACHE_CHECK([for an appropriate signed 32 bit integer type], [xerces_cv_type_s32bit_int], [
		AS_IF([test x$ac_cv_header_inttypes_h = xyes && test x$ac_cv_type_int32_t = xyes],
			[xerces_cv_type_s32bit_int=int32_t],
			[
				case $ac_cv_sizeof_int in
				4*) xerces_cv_type_s32bit_int=int;;
				*)  
					case $ac_cv_sizeof_long in
					4*) xerces_cv_type_s32bit_int=long;;
					*)  AC_MSG_ERROR([Couldn't find a signed 32 bit int type]);;
					esac
					;;
				esac
			])
	])
	
	#
	# Select an unsigned 32 bit integer type
	#
	AC_CACHE_CHECK([for an appropriate unsigned 32 bit integer type], [xerces_cv_type_u32bit_int], [
		AS_IF([test x$ac_cv_header_inttypes_h = xyes && test x$ac_cv_type_uint32_t = xyes],
			[xerces_cv_type_u32bit_int=uint32_t],
			[
				case $ac_cv_sizeof_int in
				4*) xerces_cv_type_u32bit_int="unsigned int";;
				*)  
					case $ac_cv_sizeof_long in
					4*) xerces_cv_type_u32bit_int="unsigned long";;
					*)  AC_MSG_ERROR([Couldn't find an unsigned 32 bit int type]);;
					esac
					;;
				esac
			])
	])		
		
	AC_DEFINE_UNQUOTED([XERCES_S16BIT_INT], $xerces_cv_type_s16bit_int, [An appropriate signed 16 bit integer type])
	AC_DEFINE_UNQUOTED([XERCES_U16BIT_INT], $xerces_cv_type_u16bit_int, [An appropriate unsigned 16 bit integer type])
	AC_DEFINE_UNQUOTED([XERCES_S32BIT_INT], $xerces_cv_type_s32bit_int, [An appropriate signed 32 bit integer type])
	AC_DEFINE_UNQUOTED([XERCES_U32BIT_INT], $xerces_cv_type_u32bit_int, [An appropriate unsigned 32 bit integer type])
	]
)

