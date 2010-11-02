/*
 * libmondai -- MONTSUQI data access library
 * Copyright (C) 2000-2004 Ogochan & JMA (Japan Medical Association).
 * Copyright (C) 2005-2009 Ogochan.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
#define	DEBUG
#define	TRACE
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<errno.h>
#ifdef	WITH_I18N
#include	<iconv.h>
#endif
#include	<glib.h>
#include	<math.h>

#define		__VALUE_DIRECT
#include	"types.h"
#include	"misc_v.h"
#include	"monstring.h"
#include	"value.h"
#include	"memory_v.h"
#include	"others.h"
#include	"getset.h"
#include	"debug.h"

//#define	BINARY_IS_BASE64

extern	int
ValueToInteger(
	ValueStruct	*val)
{
	int		ret;

	if		(  val  ==  NULL  ) {
		ret = 0;
	} else
	switch	(ValueType(val)) {
	  case	GL_TYPE_CHAR:
	  case	GL_TYPE_VARCHAR:
	  case	GL_TYPE_TEXT:
	  case	GL_TYPE_SYMBOL:
		ret = StrToInt(ValueString(val),strlen(ValueString(val)));
		break;
	  case	GL_TYPE_NUMBER:
		ret = FixedToInt(&ValueFixed(val));
		break;
	  case	GL_TYPE_INT:
		ret = ValueInteger(val);
		break;
	  case	GL_TYPE_FLOAT:
		ret = (int)ValueFloat(val);
		break;
	  case	GL_TYPE_BOOL:
		ret = ValueBool(val);
		break;
	  case	GL_TYPE_OBJECT:
		ret = ValueObjectId(val);
		break;
	  case	GL_TYPE_TIMESTAMP:
	  case	GL_TYPE_DATE:
	  case	GL_TYPE_TIME:
		ret = (int)mktime(&ValueDateTime(val));
		break;
	  default:
		ret = 0;
	}
	return	(ret);
}

extern	double
ValueToFloat(
	ValueStruct	*val)
{
	double	ret;

	if		(  val  ==  NULL  ) {
		ret = 0;
	} else
	switch	(ValueType(val)) {
	  case	GL_TYPE_CHAR:
	  case	GL_TYPE_VARCHAR:
	  case	GL_TYPE_TEXT:
	  case	GL_TYPE_SYMBOL:
		ret = atof(ValueString(val));
		break;
	  case	GL_TYPE_NUMBER:
		ret = FixedToFloat(&ValueFixed(val));
		break;
	  case	GL_TYPE_INT:
		ret = (double)ValueInteger(val);
		break;
	  case	GL_TYPE_FLOAT:
		ret = ValueFloat(val);
		break;
	  case	GL_TYPE_BOOL:
		ret = (double)ValueBool(val);
		break;
	  case	GL_TYPE_TIMESTAMP:
	  case	GL_TYPE_DATE:
	  case	GL_TYPE_TIME:
		ret = (double)mktime(&ValueDateTime(val));
		break;
	  default:
		ret = 0;
	}
	return	(ret);
}

extern	Fixed	*
ValueToFixed(
	ValueStruct	*val)
{
	Fixed	*ret;
	Fixed	*xval;

	if		(  val  ==  NULL  ) {
		ret = NewFixed(0,0);
	} else
	switch	(ValueType(val)) {
	  case	GL_TYPE_CHAR:
	  case	GL_TYPE_VARCHAR:
	  case	GL_TYPE_TEXT:
	  case	GL_TYPE_SYMBOL:
		ret = NewFixed(0,0);
		IntToFixed(ret,StrToInt(ValueString(val),strlen(ValueString(val))));
		break;
	  case	GL_TYPE_NUMBER:
		xval = &ValueFixed(val);
		ret = NewFixed(xval->flen,xval->slen);
		strcpy(ret->sval,xval->sval);
		break;
	  case	GL_TYPE_INT:
		ret = NewFixed(0,0);
		IntToFixed(ret,ValueInteger(val));
		break;
	  case	GL_TYPE_FLOAT:
		ret = NewFixed(SIZE_NUMBUF,(SIZE_NUMBUF / 2));
		FloatToFixed(ret,ValueFloat(val));
		break;
	  case	GL_TYPE_BOOL:
		ret = NewFixed(0,0);
		IntToFixed(ret,(int)ValueBool(val));
		break;
	  case	GL_TYPE_TIMESTAMP:
		ret = NewFixed(14,0);
		sprintf(ret->sval,"%04d%02d%02d%02d%02d%02d",
				ValueDateTimeYear(val),
				ValueDateTimeMon(val) + 1,
				ValueDateTimeMDay(val),
				ValueDateTimeHour(val),
				ValueDateTimeMin(val),
				ValueDateTimeSec(val));
		break;
	  case	GL_TYPE_DATE:
		ret = NewFixed(8,0);
		sprintf(ret->sval,"%04d%02d%02d",
				ValueDateTimeYear(val),
				ValueDateTimeMon(val) + 1,
				ValueDateTimeMDay(val));
		break;
	  case	GL_TYPE_TIME:
		ret = NewFixed(6,0);
		sprintf(ret->sval,"%02d%02d%02d",
				ValueDateTimeHour(val),
				ValueDateTimeMin(val),
				ValueDateTimeSec(val));
		break;
	  default:
		ret = NewFixed(0,0);
		break;
	}
	return	(ret);
}

extern	Bool
ValueToBool(
	ValueStruct	*val)
{
	Bool	ret;

	if		(  val  ==  NULL  ) {
		ret = FALSE;
	} else
	switch	(ValueType(val)) {
	  case	GL_TYPE_CHAR:
	  case	GL_TYPE_VARCHAR:
	  case	GL_TYPE_TEXT:
	  case	GL_TYPE_SYMBOL:
		ret = ( *ValueString(val) == 'T' ) ? TRUE : FALSE;
		break;
	  case	GL_TYPE_NUMBER:
		ret = FixedToInt(&ValueFixed(val)) ? TRUE : FALSE;
		break;
	  case	GL_TYPE_INT:
		ret = ValueInteger(val) ? TRUE : FALSE;
		break;
	  case	GL_TYPE_FLOAT:
		ret = (int)ValueFloat(val) ? TRUE : FALSE;
		break;
	  case	GL_TYPE_BOOL:
		ret = ValueBool(val);
		break;
	  default:
		ret = FALSE;
	}
	return	(ret);
}

extern	LargeByteString	*
ValueToLBS(
	ValueStruct	*val,
	char		*codeset)
{
	byte	work[SIZE_NUMBUF+1];
	byte	work2[SIZE_NUMBUF+2];
	byte	*p
		,	*q;
	int		i;
	int		size;
	LargeByteString	*ret;

ENTER_FUNC;
	if		(  val  ==  NULL  ) {
		ret = NULL;
	} else {
		dbgprintf("type = %X\n",(int)ValueType(val));
		if		(  ValueStr(val)  ==  NULL  ) {
			ValueStr(val) = NewLBS();
		}
		LBS_EmitStart(ValueStr(val));
		if		(  IS_VALUE_NIL(val)  ) {
			LBS_EmitChar(ValueStr(val),CHAR_NIL);
		} else
		switch	(ValueType(val)) {
		  case	GL_TYPE_CHAR:
		  case	GL_TYPE_VARCHAR:
		  case	GL_TYPE_TEXT:
		  case	GL_TYPE_SYMBOL:
		  case	GL_TYPE_DBCODE:
			if		(  ValueString(val)  !=  NULL  ) {
				RewindLBS(ValueStr(val));
				if		(  IS_VALUE_EXPANDABLE(val)  ) {
					LBS_EmitStringCodeset(ValueStr(val),ValueString(val),
										  ValueStringSize(val),
										  ValueStringLength(val),codeset);
					if		(  ( size = ValueStringLength(val) - ValueSize(val) )  >  0  ) {
						for	(  ; size > 0 ; size -- ) {
							LBS_EmitChar(ValueStr(val),0);
						}
					}
				} else {
					LBS_EmitStringCodeset(ValueStr(val),ValueString(val),
										  ValueStringSize(val),
										  0,codeset);
				}
			}
			break;
		  case	GL_TYPE_BYTE:
		  case	GL_TYPE_BINARY:
#ifdef	BINARY_IS_BASE64
			size = ( ( ValueByteLength(val) + 2 ) / 3 ) * 4;
			p = (char *)xmalloc(size);
			size = EncodeBase64(p,size,ValueByte(val),ValueByteLength(val));
			LBS_ReserveSize(ValueStr(val),size+1);
			strcpy(ValueStrBody(val),p);
			xfree(p);
#else
			p = ValueByte(val);
			for	( i = 0 ; i < ValueByteLength(val) ; i ++ , p ++ ) {
				switch	(*p) {
				  case	'\\':
					LBS_EmitChar(ValueStr(val),'\\');
					LBS_EmitChar(ValueStr(val),'\\');
					break;
				  case	'"':
					LBS_EmitChar(ValueStr(val),'\\');
					LBS_EmitChar(ValueStr(val),'"');
					break;
				  case	'/':
					LBS_EmitChar(ValueStr(val),'\\');
					LBS_EmitChar(ValueStr(val),'/');
					break;
				  case	'\b':
					LBS_EmitChar(ValueStr(val),'\\');
					LBS_EmitChar(ValueStr(val),'b');
					break;
				  case	'\f':
					LBS_EmitChar(ValueStr(val),'\\');
					LBS_EmitChar(ValueStr(val),'f');
					break;
				  case	'\n':
					LBS_EmitChar(ValueStr(val),'\\');
					LBS_EmitChar(ValueStr(val),'n');
					break;
				  case	'\r':
					LBS_EmitChar(ValueStr(val),'\\');
					LBS_EmitChar(ValueStr(val),'r');
					break;
				  case	'\t':
					LBS_EmitChar(ValueStr(val),'\\');
					LBS_EmitChar(ValueStr(val),'t');
					break;
				  default:
					if		(  isprint(*p)  ) {
						LBS_EmitChar(ValueStr(val),*p);
					} else {
						sprintf(work,"\\u%02X",(int)*p);
						LBS_EmitString(ValueStr(val),work);
					}
					break;
				}
			}
			if		(  ( size = ValueStringLength(val) - ValueSize(val) )  >  0  ) {
				for	(  ; size > 0 ; size -- ) {
					LBS_EmitByte(ValueStr(val),0);
				}
			}
#endif
			break;
		  case	GL_TYPE_NUMBER:
			strcpy(work,ValueFixedBody(val));
			p = work;
			q = work2;
			if		(  *p  >=  0x70  ) {
				*q ++ = '-';
				*p ^= 0x40;
			}
			strcpy(q,p);
			if		(  ValueFixedSlen(val)  >  0  ) {
				p = work2 + strlen(work2);
				*(p + 1) = 0;
				q = p - 1;
				for	( i = 0 ; i < ValueFixedSlen(val) ; i ++ ) {
					*p -- = *q --;
				}
				*p = '.';
			}
			LBS_EmitString(ValueStr(val),work2);
			break;
		  case	GL_TYPE_INT:
			sprintf(work,"%d",ValueInteger(val));
			LBS_EmitString(ValueStr(val),work);
			break;
		  case	GL_TYPE_OBJECT:
			sprintf(work,"%d",(int)ValueObjectId(val));
			LBS_EmitString(ValueStr(val),work);
			break;
		  case	GL_TYPE_FLOAT:
			sprintf(work,"%g",ValueFloat(val));
			LBS_EmitString(ValueStr(val),work);
			break;
		  case	GL_TYPE_BOOL:
			sprintf(work,"%s",ValueBool(val) ? "TRUE" : "FALSE");
			LBS_EmitString(ValueStr(val),work);
			break;
		  case	GL_TYPE_TIMESTAMP:
			sprintf(work,"%04d%02d%02d%02d%02d%02d",
					ValueDateTimeYear(val),
					ValueDateTimeMon(val) + 1,
					ValueDateTimeMDay(val),
					ValueDateTimeHour(val),
					ValueDateTimeMin(val),
					ValueDateTimeSec(val));
			LBS_EmitString(ValueStr(val),work);
			break;
		  case	GL_TYPE_DATE:
			sprintf(work,"%04d%02d%02d",
					ValueDateTimeYear(val),
					ValueDateTimeMon(val) + 1,
					ValueDateTimeMDay(val));
			LBS_EmitString(ValueStr(val),work);
			break;
		  case	GL_TYPE_TIME:
			sprintf(work,"%02d%02d%02d",
					ValueDateTimeHour(val),
					ValueDateTimeMin(val),
					ValueDateTimeSec(val));
			LBS_EmitString(ValueStr(val),work);
			break;
		  default:
			break;
		}
		LBS_EmitEnd(ValueStr(val));
		ret = ValueStr(val);
	}
LEAVE_FUNC;
	return	(ret);
}

extern	char	*
ValueToString(
	ValueStruct	*val,
	char		*codeset)
{
	char	*ret;

	if		(  ValueToLBS(val,codeset)  ==  NULL  ) {
		ret = NULL;
	} else {
		ret = ValueStrBody(val);
	}
	return	(ret);
}

static	void
DecodeStringToBinary(
	byte	*p,
	size_t	size,
	char	*str)
{
#ifdef	BINARY_IS_BASE64
	DecodeBase64(p,size,str,strlen(str));
#else
	int		i;

	for	( i = 0 ; i < size ; i ++ , p ++ ) {
		if		(  *str  ==  '\\'  ) {
			str ++;
			switch	(*str) {
			  case	'b':
				*p = '\b';
				str ++;
				break;
			  case	'f':
				*p = '\f';
				str ++;
				break;
			  case	'n':
				*p = '\n';
				str ++;
				break;
			  case	'r':
				*p = '\r';
				str ++;
				break;
			  case	't':
				*p = '\t';
				str ++;
				break;
			  case	'u':
				str ++;
				*p = (unsigned char)HexToInt(str,2);
				str += 2;
				break;
			  default:
				*p = *str;
				str ++;
				break;
			}
		} else {
			*p = *str;
			str ++;
		}
	}
#endif
}

extern	Bool
SetValueStringWithLength(
	ValueStruct	*val,
	char		*str,
	size_t		slen,
	char		*codeset)
{
	Bool	rc
	,		fMinus
	,		fPoint;
	size_t	len;
	byte	*p;
	byte	buff[SIZE_NUMBUF+1]
		,	sbuff[SIZE_LONGNAME+1];
	Fixed	from;
	size_t	size;
#ifdef	WITH_I18N
	byte	*q;
	iconv_t	cd;
	size_t	sob
	,		sib;
	char	*istr;
#endif

ENTER_FUNC;
	if		(  val  ==  NULL  ) {
		fprintf(stderr,"no ValueStruct\n");
		rc = FALSE;
	} else
	if		(	(  str   ==  NULL      )
			||	(  *str  ==  CHAR_NIL  ) ) {
		ValueIsNil(val);
		rc = TRUE;
	} else {
		ValueIsNonNil(val);
		switch	(ValueType(val)) {
		  case	GL_TYPE_CHAR:
		  case	GL_TYPE_VARCHAR:
		  case	GL_TYPE_DBCODE:
		  case	GL_TYPE_TEXT:
		  case	GL_TYPE_SYMBOL:
#ifdef	WITH_I18N
			if		(	(  codeset  !=  NULL  )
					&&	(	(  stricmp(codeset,"utf8")   ==  0  )
						||	(  stricmp(codeset,"utf-8")  ==  0  ) ) )	{
				codeset = NULL;
			}
			if		(  codeset  !=  NULL  ) {
				if		(  IS_VALUE_EXPANDABLE(val)  ) {
					len = slen;
				} else {
					len = ValueStringLength(val) < slen ?
						ValueStringLength(val) : slen;
				}
				cd = iconv_open("utf8",codeset);
				while	(TRUE) {
					istr = str;
					sib = len;
					sob = ValueStringSize(val);
					if		(  ( q = ValueString(val) )  !=  NULL  ) {
						memclear(ValueString(val),ValueStringSize(val));
						if		(  iconv(cd,&istr,&sib,(void*)&q,&sob)  ==  0  )	break;
						if		(  errno  ==  E2BIG ) {
							xfree(ValueString(val));
							ValueStringSize(val) *= 2;
						} else
							break;
					} else {
						ValueStringSize(val) = 1;
					}
					ValueString(val) = (byte *)xmalloc(ValueStringSize(val));
				};
				iconv_close(cd);
				*q = 0;
				len = ValueStringSize(val) - sob;
			} else {
#endif
				size = slen + 1;
				len = slen;
				if		(  size  >  ValueStringSize(val)  ) {
					if		(  ValueString(val)  !=  NULL  ) {
						xfree(ValueString(val));
					}
					ValueStringSize(val) = size;
					ValueString(val) = (byte *)xmalloc(size);
				}
				memclear(ValueString(val),ValueStringSize(val));
				strcpy(ValueString(val),str);
#ifdef	WITH_I18N
			}
#endif
			if		(  IS_VALUE_EXPANDABLE(val)  ) {
				ValueStringLength(val) = len;
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_BYTE:
			DecodeStringToBinary(ValueByte(val),ValueByteLength(val),str);
			rc = TRUE;
			break;
		  case	GL_TYPE_BINARY:
#ifdef	BINARY_IS_BASE64
			p = (byte *)xmalloc(strlen(str));
			size = strlen(str);
			size = DecodeBase64(p,size,str,size);
			if		(  ValueByte(val)  !=  NULL  ) {
				xfree(ValueByte(val));
			}
			ValueByteSize(val) = strlen(str);
			ValueByte(val) = p;
#else
			size = 0;
			for	( p = str ; *p != 0 ; ) {
				if		(  *p  ==  '\\'  ) {
					p ++;
					if		(  *p  ==  'u'  ) {
						p += 3;
					} else {
						p ++;
					}
				} else {
					p ++;
				}
				size ++;
			}
			if		(  size  >  ValueByteSize(val)  ) {
				if		(  ValueByte(val)  !=  NULL  ) {
						xfree(ValueByte(val));
				}
				ValueByteSize(val) = size;
				ValueByte(val) = (byte *)xmalloc(size);
			}
			memclear(ValueByte(val),ValueByteSize(val));
			DecodeStringToBinary(ValueByte(val),size,str);
#endif
			ValueByteLength(val) = size;
			rc = TRUE;
			break;
		  case	GL_TYPE_OBJECT:
			ValueObjectId(val) = 0;
			for	( ; slen > 0 ; slen -- )	{
				if		(  isdigit(*str)  )	{
					ValueObjectId(val) = ValueObjectId(val) * 10 + ( *str - '0' );
				}
				str ++;
			}
			if		(  ValueObjectFile(val)  !=  NULL  ) {
				xfree(ValueObjectFile(val));
				ValueObjectFile(val) = NULL;
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_NUMBER:
		  case	GL_TYPE_INT:
		  case	GL_TYPE_FLOAT:
		  case	GL_TYPE_BOOL:
		  case	GL_TYPE_TIMESTAMP:
		  case	GL_TYPE_DATE:
		  case	GL_TYPE_TIME:
#ifdef	WITH_I18N
			if		(  codeset  !=  NULL  ) {
				cd = iconv_open("utf8",codeset);
				istr = str;
				sib = slen;
				sob = SIZE_NUMBUF;
				p = sbuff;
				iconv(cd,&istr,&sib,(void*)&p,&sob);
				iconv_close(cd);
				*p = 0;
				str = sbuff;
			} else {
#endif
				strncpy(sbuff,str,SIZE_NUMBUF);
				str = sbuff;
#ifdef	WITH_I18N
			}
#endif
			switch	(ValueType(val)) {
			  case	GL_TYPE_NUMBER:
				p = buff;
				from.flen = 0;
				from.slen = 0;
				from.sval = buff;
				fPoint = FALSE;
				fMinus = FALSE;
				while	(  *str  !=  0  ) {
					if		(  fPoint  ) {
						from.slen ++;
					}
					if		(  *str  ==  '-'  ) {
						fMinus = TRUE;
					} else
					if	(  isdigit(*str)  ) {
						*p ++ = *str;
						from.flen ++;
					} else
					if		(  *str  ==  '.'  ) {
						fPoint = TRUE;
					}
					str ++;
				}
				*p = 0;
				if		(  fMinus  ) {
					*buff |= 0x40;
				}
				FixedRescale(&ValueFixed(val),&from);
				rc = TRUE;
				break;
			  case	GL_TYPE_INT:
				ValueInteger(val) = StrToInt(str,slen);
				rc = TRUE;
				break;
			  case	GL_TYPE_FLOAT:
				ValueFloat(val) = atof(str);
				rc = TRUE;
				break;
			  case	GL_TYPE_BOOL:
				ValueBool(val) = ( toupper(*str) == 'T' ) ? TRUE : FALSE;
				rc = TRUE;
				break;
			  case	GL_TYPE_TIMESTAMP:
				ValueDateTimeYear(val) = StrToInt(str,4);		str += 4;
				ValueDateTimeMon(val) = StrToInt(str,2) - 1;	str += 2;
				ValueDateTimeMDay(val) = StrToInt(str,2);		str += 2;
				ValueDateTimeHour(val) = StrToInt(str,2);		str += 2;
				ValueDateTimeMin(val) = StrToInt(str,2);		str += 2;
				ValueDateTimeSec(val) = StrToInt(str,2);		str += 2;
				rc = mktime(&ValueDateTime(val)) >= 0 ? TRUE : FALSE;
				break;
			  case	GL_TYPE_DATE:
				ValueDateTimeYear(val) = StrToInt(str,4);		str += 4;
				ValueDateTimeMon(val) = StrToInt(str,2) - 1;	str += 2;
				ValueDateTimeMDay(val) = StrToInt(str,2);		str += 2;
				ValueDateTimeHour(val) = 0;
				ValueDateTimeMin(val) = 0;
				ValueDateTimeSec(val) = 0;
				rc = mktime(&ValueDateTime(val)) >= 0 ? TRUE : FALSE;
				break;
			  case	GL_TYPE_TIME:
				ValueDateTimeYear(val) = 0;
				ValueDateTimeMon(val) = 0;
				ValueDateTimeMDay(val) = 0;
				ValueDateTimeHour(val) = StrToInt(str,2);	str += 2;
				ValueDateTimeMin(val) = StrToInt(str,2);	str += 2;
				ValueDateTimeSec(val) = StrToInt(str,2);	str += 2;
				rc = TRUE;
				break;
			  default:
				rc = TRUE;
				break;
			}
			break;
		  default:
			ValueIsNil(val);
			rc = FALSE;	  
		}
	}
LEAVE_FUNC;
	return	(rc);
}

extern	Bool
SetValueInteger(
	ValueStruct	*val,
	int			ival)
{
	Bool	rc;
	char	str[SIZE_NUMBUF+1];
	Bool	fMinus;

	if		(  val  ==  NULL  ) {
		fprintf(stderr,"no ValueStruct\n");
		rc = FALSE;
	} else {
		ValueIsNonNil(val);
		switch	(ValueType(val)) {
		  case	GL_TYPE_CHAR:
		  case	GL_TYPE_VARCHAR:
		  case	GL_TYPE_DBCODE:
		  case	GL_TYPE_TEXT:
		  case	GL_TYPE_SYMBOL:
			sprintf(str,"%d",ival);
			rc = SetValueString(val,str,NULL);
			break;
		  case	GL_TYPE_NUMBER:
			if		(  ival  <  0  ) {
				ival = - ival;
				fMinus = TRUE;
			} else {
				fMinus = FALSE;
			}
			sprintf(str,"%0*d",(int)ValueFixedLength(val),ival);
			if		(  fMinus  ) {
				*str |= 0x40;
			}
			rc = SetValueString(val,str,NULL);
			break;
		  case	GL_TYPE_INT:
			ValueInteger(val) = ival;
			rc = TRUE;
			break;
		  case	GL_TYPE_FLOAT:
			ValueFloat(val) = ival;
			rc = TRUE;
			break;
		  case	GL_TYPE_BOOL:
			ValueBool(val) = ( ival == 0 ) ? FALSE : TRUE;
			rc = TRUE;
			break;
		  case	GL_TYPE_OBJECT:
			ValueObjectId(val) = ival;
			if		(  ValueObjectFile(val)  !=  NULL  ) {
				xfree(ValueObjectFile(val));
				ValueObjectFile(val) = NULL;
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_TIMESTAMP:
		  case	GL_TYPE_DATE:
		  case	GL_TYPE_TIME:
			rc = (  localtime_r((time_t *)&ival,&ValueDateTime(val))  !=  NULL  ) ? TRUE : FALSE;
			break;
		  default:
			ValueIsNil(val);
			rc = FALSE;	  
		}
	}
	return	(rc);
}

extern	Bool
SetValueChar(
	ValueStruct	*val,
	char		cval)
{
	Bool	rc;
	char	str[SIZE_NUMBUF+1];

	if		(  val  ==  NULL  ) {
		fprintf(stderr,"no ValueStruct\n");
		rc = FALSE;
	} else {
		ValueIsNonNil(val);
		switch	(ValueType(val)) {
		  case	GL_TYPE_CHAR:
		  case	GL_TYPE_VARCHAR:
		  case	GL_TYPE_DBCODE:
		  case	GL_TYPE_TEXT:
		  case	GL_TYPE_SYMBOL:
			sprintf(str,"%c",cval);
			rc = SetValueString(val,str,NULL);
			break;
		  case	GL_TYPE_NUMBER:
			sprintf(str,"%0*d",(int)ValueFixedLength(val),(int)cval);
			rc = SetValueString(val,str,NULL);
			break;
		  case	GL_TYPE_INT:
			ValueInteger(val) = (int)cval;
			rc = TRUE;
			break;
		  case	GL_TYPE_FLOAT:
			ValueFloat(val) = (double)cval;
			rc = TRUE;
			break;
		  case	GL_TYPE_BOOL:
			ValueBool(val) = ( cval == 0 ) ? FALSE : TRUE;
			rc = TRUE;
			break;
		  default:
			ValueIsNil(val);
			rc = FALSE;	  
		}
	}
	return	(rc);
}

extern	Bool
SetValueBool(
	ValueStruct	*val,
	Bool		bval)
{
	Bool	rc;
	char	str[SIZE_NUMBUF+1];

	if		(  val  ==  NULL  ) {
		fprintf(stderr,"no ValueStruct\n");
		rc = FALSE;
	} else {
		ValueIsNonNil(val);
		switch	(ValueType(val)) {
		  case	GL_TYPE_CHAR:
		  case	GL_TYPE_VARCHAR:
		  case	GL_TYPE_DBCODE:
		  case	GL_TYPE_TEXT:
		  case	GL_TYPE_SYMBOL:
			sprintf(str,"%s",(bval ? "TRUE" : "FALSE"));
			rc = SetValueString(val,str,NULL);
			break;
		  case	GL_TYPE_NUMBER:
			sprintf(str,"%d",bval);
			rc = SetValueString(val,str,NULL);
			break;
		  case	GL_TYPE_INT:
			ValueInteger(val) = bval;
			rc = TRUE;
			break;
		  case	GL_TYPE_FLOAT:
			ValueFloat(val) = bval;
			rc = TRUE;
			break;
		  case	GL_TYPE_BOOL:
			ValueBool(val) = bval;
			rc = TRUE;
			break;
		  default:
			ValueIsNil(val);
			rc = FALSE;
			break;
		}
	}
	return	(rc);
}

extern	Bool
SetValueFloat(
	ValueStruct	*val,
	double		fval)
{
	time_t	wt;
	Bool	rc;
	char	str[SIZE_NUMBUF+1];

	if		(  val  ==  NULL  ) {
		fprintf(stderr,"no ValueStruct\n");
		rc = FALSE;
	} else {
		ValueIsNonNil(val);
		switch	(ValueType(val)) {
		  case	GL_TYPE_CHAR:
		  case	GL_TYPE_VARCHAR:
		  case	GL_TYPE_DBCODE:
		  case	GL_TYPE_TEXT:
		  case	GL_TYPE_SYMBOL:
			sprintf(str,"%f",fval);
			rc = SetValueString(val,str,NULL);
			break;
		  case	GL_TYPE_NUMBER:
			FloatToFixed(&ValueFixed(val),fval);
			rc = TRUE;
			break;
		  case	GL_TYPE_INT:
			ValueInteger(val) = (int)fval;
			rc = TRUE;
			break;
		  case	GL_TYPE_FLOAT:
			ValueFloat(val) = fval;
			rc = TRUE;
			break;
		  case	GL_TYPE_BOOL:
			ValueBool(val) = ( fval == 0 ) ? FALSE : TRUE;
			rc = TRUE;
			break;
		  case	GL_TYPE_TIMESTAMP:
		  case	GL_TYPE_DATE:
		  case	GL_TYPE_TIME:
			wt = (time_t)fval;
			rc = (  localtime_r(&wt,&ValueDateTime(val))  !=  NULL  ) ? TRUE : FALSE;
			break;
		  default:
			ValueIsNil(val);
			rc = FALSE;
			break;
		}
	}
	return	(rc);
}

extern	Bool
SetValueFixed(
	ValueStruct	*val,
	Fixed		*fval)
{
	Bool	rc;
	char	*str;

	if		(  val  ==  NULL  ) {
		fprintf(stderr,"no ValueStruct\n");
		rc = FALSE;
	} else {
		ValueIsNonNil(val);
		switch	(ValueType(val)) {
		  case	GL_TYPE_CHAR:
		  case	GL_TYPE_VARCHAR:
		  case	GL_TYPE_DBCODE:
		  case	GL_TYPE_TEXT:
		  case	GL_TYPE_SYMBOL:
			rc = SetValueString(val,fval->sval,NULL);
			break;
		  case	GL_TYPE_NUMBER:
			FixedRescale(&ValueFixed(val),fval);
			rc = TRUE;
			break;
		  case	GL_TYPE_INT:
			ValueInteger(val) = FixedToInt(fval);
			rc = TRUE;
			break;
		  case	GL_TYPE_FLOAT:
			ValueFloat(val) = FixedToFloat(fval);
			rc = TRUE;
			break;
		  case	GL_TYPE_TIMESTAMP:
			str = fval->sval;
			ValueDateTimeYear(val) = StrToInt(str,4);		str += 4;
			ValueDateTimeMon(val) = StrToInt(str,2) - 1;	str += 2;
			ValueDateTimeMDay(val) = StrToInt(str,2);		str += 2;
			ValueDateTimeHour(val) = StrToInt(str,2);		str += 2;
			ValueDateTimeMin(val) = StrToInt(str,2);		str += 2;
			ValueDateTimeSec(val) = StrToInt(str,2);		str += 2;
			rc = mktime(&ValueDateTime(val)) >= 0 ? TRUE : FALSE;
			break;
		  case	GL_TYPE_DATE:
			str = fval->sval;
			ValueDateTimeYear(val) = StrToInt(str,4);		str += 4;
			ValueDateTimeMon(val) = StrToInt(str,2) - 1;	str += 2;
			ValueDateTimeMDay(val) = StrToInt(str,2);		str += 2;
			ValueDateTimeHour(val) = 0;
			ValueDateTimeMin(val) = 0;
			ValueDateTimeSec(val) = 0;
			rc = mktime(&ValueDateTime(val)) >= 0 ? TRUE : FALSE;
			break;
		  case	GL_TYPE_TIME:
			str = fval->sval;
			ValueDateTimeYear(val) = 0;
			ValueDateTimeMon(val) = 0;
			ValueDateTimeMDay(val) = 0;
			ValueDateTimeHour(val) = StrToInt(str,2);	str += 2;
			ValueDateTimeMin(val) = StrToInt(str,2);	str += 2;
			ValueDateTimeSec(val) = StrToInt(str,2);	str += 2;
			rc = TRUE;
			break;
#if	0
		  case	GL_TYPE_BOOL:
			val->body.BoolData = ( *fval->sval == 0 ) ? FALSE : TRUE;
			rc = TRUE;
			break;
#endif
		  default:
			ValueIsNil(val);
			rc = FALSE;
			break;
		}
	}
	return	(rc);
}

extern	Bool
SetValueBinary(
	ValueStruct	*val,
	byte		*str,
	size_t		slen)
{
	Bool	rc;
	size_t	size;

ENTER_FUNC;
	if		(  val  ==  NULL  ) {
		fprintf(stderr,"no ValueStruct\n");
		rc = FALSE;
	} else {
		if		(	(  str   ==  NULL      )
				||	(  slen  ==  0         ) ) {
			ValueIsNil(val);
			rc = TRUE;
		} else {
			ValueIsNonNil(val);
		}
		switch	(ValueType(val)) {
		  case	GL_TYPE_CHAR:
		  case	GL_TYPE_VARCHAR:
		  case	GL_TYPE_DBCODE:
		  case	GL_TYPE_TEXT:
		  case	GL_TYPE_SYMBOL:
			if		(  *str  ==  CHAR_NIL  ) {
				ValueIsNil(val);
			} else {
				size = slen + 1;
				if		(  size  >  ValueStringSize(val)  ) {
					if		(  ValueString(val)  !=  NULL  ) {
						xfree(ValueString(val));
					}
					ValueStringSize(val) = size;
					ValueString(val) = (byte *)xmalloc(size);
				}
				memclear(ValueString(val),ValueStringSize(val));
				if		(  str  !=  NULL  ) {
					strcpy(ValueString(val),str);
				}
				if		(  ValueType(val)  ==  GL_TYPE_TEXT  ) {
					ValueStringLength(val) = slen;
				}
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_BYTE:
			size = ValueByteLength(val) < slen ? ValueByteLength(val) : slen;
			memclear(ValueByte(val),ValueByteSize(val));
			if		(  str  !=  NULL  ) {
				memcpy(ValueByte(val),str,size);
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_BINARY:
			if		(  slen  >  ValueByteSize(val)  ) {
				if		(  ValueByte(val)  !=  NULL  ) {
					xfree(ValueByte(val));
				}
				ValueByteSize(val) = slen;
				ValueByte(val) = (byte *)xmalloc(slen);
			}
			memclear(ValueByte(val),ValueByteSize(val));
			if		(  str  !=  NULL  ) {
				memcpy(ValueByte(val),str,slen);
			}
			ValueByteLength(val) = slen;
			rc = TRUE;
			break;
		  case	GL_TYPE_NUMBER:
			if		(  str  !=  NULL  ) {
				memcpy(&ValueFixed(val),str,sizeof(Fixed));
			} else {
				memclear(&ValueFixed(val),sizeof(Fixed));
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_INT:
			if		(  str  !=  NULL  ) {
				memcpy(&ValueInteger(val),str,sizeof(int));
			} else {
				memclear(&ValueInteger(val),sizeof(int));
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_OBJECT:
			if		(  str  !=  NULL  ) {
				memcpy(&ValueObjectId(val),str,sizeof(MonObjectType));
			} else {
				ValueObjectId(val) = 0;
			}
			if		(  ValueObjectFile(val)  !=  NULL  ) {
				xfree(ValueObjectFile(val));
				ValueObjectFile(val) = NULL;
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_FLOAT:
			if		(  str  !=  NULL  ) {
				memcpy(&ValueFloat(val),str,sizeof(double));
			} else {
				memclear(&ValueFloat(val),sizeof(double));
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_BOOL:
			if		(  str  !=  NULL  ) {
				memcpy(&ValueBool(val),str,sizeof(Bool));
			} else {
				memclear(&ValueBool(val),sizeof(Bool));
			}
			rc = TRUE;
			break;
		  case	GL_TYPE_TIMESTAMP:
		  case	GL_TYPE_DATE:
		  case	GL_TYPE_TIME:
			if		(  str  !=  NULL  ) {
				memcpy(&ValueDateTime(val),str,sizeof(struct tm));
				rc = mktime(&ValueDateTime(val)) >= 0 ? TRUE : FALSE;
			} else {
				memclear(&ValueDateTime(val),sizeof(struct tm));
				rc = TRUE;
			}
			break;
		  default:
			ValueIsNil(val);
			rc = FALSE;
			break;
		}
	}
LEAVE_FUNC;
	return	(rc);
}

extern	byte	*
ValueToBinary(
	ValueStruct	*val)
{
	byte	*ret;
ENTER_FUNC;

	if		(  val  ==  NULL  ) {
		ret = NULL;
	} else {
		dbgprintf("type = %X\n",(int)ValueType(val));
		if		(  ValueStr(val)  ==  NULL  ) {
			ValueStr(val) = NewLBS();
		}
		LBS_EmitStart(ValueStr(val));
		if		(  IS_VALUE_NIL(val)  ) {
			LBS_EmitChar(ValueStr(val),CHAR_NIL);
		} else {
			switch	(ValueType(val)) {
			  case	GL_TYPE_CHAR:
			  case	GL_TYPE_VARCHAR:
			  case	GL_TYPE_DBCODE:
			  case	GL_TYPE_TEXT:
			  case	GL_TYPE_SYMBOL:
				if		(  ValueString(val)  !=  NULL  ) {
					LBS_ReserveSize(ValueStr(val),strlen(ValueString(val))+1,FALSE);
					strcpy(ValueStrBody(val),ValueString(val));
				} else {
					LBS_EmitChar(ValueStr(val),CHAR_NIL);
				}
				break;
			  case	GL_TYPE_BYTE:
			  case	GL_TYPE_BINARY:
				LBS_ReserveSize(ValueStr(val),ValueByteLength(val),FALSE);
				memcpy(ValueStrBody(val),ValueByte(val),ValueByteLength(val));
				break;
			  case	GL_TYPE_NUMBER:
				LBS_ReserveSize(ValueStr(val),sizeof(Fixed),FALSE);
				memcpy(ValueStrBody(val),&ValueFixed(val),sizeof(Fixed));
				break;
			  case	GL_TYPE_INT:
				LBS_ReserveSize(ValueStr(val),sizeof(int),FALSE);
				memcpy(ValueStrBody(val),&ValueInteger(val),sizeof(int));
				break;
			  case	GL_TYPE_OBJECT:
				LBS_ReserveSize(ValueStr(val),sizeof(MonObjectType),FALSE);
				memcpy(ValueStrBody(val),&ValueObject(val),sizeof(MonObjectType));
				break;
			  case	GL_TYPE_FLOAT:
				LBS_ReserveSize(ValueStr(val),sizeof(double),FALSE);
				memcpy(ValueStrBody(val),&ValueFloat(val),sizeof(double));
				break;
			  case	GL_TYPE_BOOL:
				LBS_ReserveSize(ValueStr(val),sizeof(Bool),FALSE);
				memcpy(ValueStrBody(val),&ValueBool(val),sizeof(Bool));
				break;
			  case	GL_TYPE_TIMESTAMP:
			  case	GL_TYPE_DATE:
			  case	GL_TYPE_TIME:
				LBS_ReserveSize(ValueStr(val),sizeof(int)*9,FALSE);
				memcpy(ValueStrBody(val),&ValueDateTime(val),sizeof(int)*9);
				break;
			  default:
				break;
			}
		}
		ret = ValueStrBody(val);
	}
LEAVE_FUNC;
	return	(ret);
}

extern	Bool
SetValueDateTime(
	ValueStruct	*val,
	struct	tm	tval)
{
	Bool	rc;
	char	str[SIZE_NUMBUF+1];

	if		(  val  ==  NULL  ) {
		fprintf(stderr,"no ValueStruct\n");
		rc = FALSE;
	} else {
		ValueIsNonNil(val);
		switch	(ValueType(val)) {
		  case	GL_TYPE_CHAR:
		  case	GL_TYPE_VARCHAR:
		  case	GL_TYPE_DBCODE:
		  case	GL_TYPE_TEXT:
		  case	GL_TYPE_SYMBOL:
		  case	GL_TYPE_NUMBER:
			sprintf(str,"%04d%02d%02d%02d%02d%02d",
					tval.tm_year,
					tval.tm_mon + 1,
					tval.tm_mday,
					tval.tm_hour,
					tval.tm_min,
					tval.tm_sec);
			rc = SetValueString(val,str,NULL);
			break;
		  case	GL_TYPE_INT:
			rc = ( ( ValueInteger(val) = (int)mktime(&tval) ) < 0 ) ? FALSE : TRUE;
			break;
		  case	GL_TYPE_FLOAT:
			rc = ( ( ValueFloat(val) = (double)mktime(&tval) ) < 0 ) ? FALSE : TRUE;
			break;
		  case	GL_TYPE_BOOL:
			rc = ( ValueBool(val) = ( mktime(&tval) == 0 ? FALSE : TRUE ) < 0 ) ? FALSE : TRUE;
			break;
		  case	GL_TYPE_TIMESTAMP:
		  case	GL_TYPE_DATE:
		  case	GL_TYPE_TIME:
			memcpy(&ValueDateTime(val),&tval,sizeof(struct tm));
			rc = (  mktime(&ValueDateTime(val))  < 0 ) ? FALSE : TRUE;
			break;
		  default:
			ValueIsNil(val);
			rc = FALSE;	  
		}
	}
	return	(rc);
}

extern	struct	tm
ValueToDateTime(
	ValueStruct	*val)
{
	time_t	wt;
	struct	tm	ret;
	Fixed	*xval;
	char	*str;

	memclear(&ret,sizeof(struct tm));
	if		(  val  ==  NULL  ) {
	} else
	switch	(ValueType(val)) {
	  case	GL_TYPE_CHAR:
	  case	GL_TYPE_VARCHAR:
	  case	GL_TYPE_TEXT:
	  case	GL_TYPE_SYMBOL:
		str = ValueString(val);
		ret.tm_year = StrToInt(str,4);		str += 4;
		ret.tm_mon = StrToInt(str,2) - 1;	str += 2;
		ret.tm_mday = StrToInt(str,2);		str += 2;
		ret.tm_hour = StrToInt(str,2);		str += 2;
		ret.tm_min = StrToInt(str,2);		str += 2;
		ret.tm_sec = StrToInt(str,2);		str += 2;
		mktime(&ret);
		break;
	  case	GL_TYPE_NUMBER:
		xval = &ValueFixed(val);
		str = xval->sval;
		ret.tm_year = StrToInt(str,4);		str += 4;
		ret.tm_mon = StrToInt(str,2) - 1;	str += 2;
		ret.tm_mday = StrToInt(str,2);		str += 2;
		ret.tm_hour = StrToInt(str,2);		str += 2;
		ret.tm_min = StrToInt(str,2);		str += 2;
		ret.tm_sec = StrToInt(str,2);		str += 2;
		mktime(&ret);
		break;
	  case	GL_TYPE_INT:
		wt = (time_t)ValueInteger(val);
		localtime_r(&wt,&ret);
		break;
	  case	GL_TYPE_FLOAT:
		wt = (time_t)ValueFloat(val);
		localtime_r(&wt,&ret);
		break;
	  case	GL_TYPE_TIMESTAMP:
		memcpy(&ret,&ValueDateTime(val),sizeof(struct tm));
		mktime(&ret);
		break;
	  case	GL_TYPE_DATE:
		memcpy(&ret,&ValueDateTime(val),sizeof(struct tm));
		ret.tm_hour = 0;
		ret.tm_min = 0;
		ret.tm_sec = 0;
		mktime(&ret);
		break;
	  case	GL_TYPE_TIME:
		memcpy(&ret,&ValueDateTime(val),sizeof(struct tm));
		mktime(&ret);
		ret.tm_year = 0;
		ret.tm_mon = 0;
		ret.tm_mday = 0;
		break;
	  default:
		break;
	}
	return	(ret);
}


extern	struct	tm
ValueToDate(
	ValueStruct	*val)
{
	struct	tm	ret;
	Fixed	*xval;
	char	*str;

	memclear(&ret,sizeof(struct tm));
	if		(  val  ==  NULL  ) {
	} else
	switch	(ValueType(val)) {
	  case	GL_TYPE_CHAR:
	  case	GL_TYPE_VARCHAR:
	  case	GL_TYPE_TEXT:
	  case	GL_TYPE_SYMBOL:
		str = ValueString(val);
		ret.tm_year = StrToInt(str,4);		str += 4;
		ret.tm_mon = StrToInt(str,2) - 1;	str += 2;
		ret.tm_mday = StrToInt(str,2);		str += 2;
		ret.tm_hour = 0;
		ret.tm_min = 0;
		ret.tm_sec = 0;
		mktime(&ret);
		break;
	  case	GL_TYPE_NUMBER:
		xval = &ValueFixed(val);
		str = xval->sval;
		ret.tm_year = StrToInt(str,4);		str += 4;
		ret.tm_mon = StrToInt(str,2) - 1;	str += 2;
		ret.tm_mday = StrToInt(str,2);		str += 2;
		ret.tm_hour = 0;
		ret.tm_min = 0;
		ret.tm_sec = 0;
		mktime(&ret);
		break;
	  case	GL_TYPE_TIMESTAMP:
	  case	GL_TYPE_DATE:
		memcpy(&ret,&ValueDateTime(val),sizeof(struct tm));
		ret.tm_hour = 0;
		ret.tm_min = 0;
		ret.tm_sec = 0;
		mktime(&ret);
		break;
	  default:
		break;
	}
	return	(ret);
}

extern	struct	tm
ValueToTime(
	ValueStruct	*val)
{
	time_t	wt;
	struct	tm	ret;
	Fixed	*xval;
	char	*str;

	memclear(&ret,sizeof(struct tm));
	if		(  val  ==  NULL  ) {
	} else
	switch	(ValueType(val)) {
	  case	GL_TYPE_CHAR:
	  case	GL_TYPE_VARCHAR:
	  case	GL_TYPE_TEXT:
	  case	GL_TYPE_SYMBOL:
		str = ValueString(val);
		ret.tm_year = 0;
		ret.tm_mon = 0;
		ret.tm_mday = 0;
		ret.tm_hour = StrToInt(str,2);	str += 2;
		ret.tm_min = StrToInt(str,2);	str += 2;
		ret.tm_sec = StrToInt(str,2);	str += 2;
		break;
	  case	GL_TYPE_NUMBER:
		xval = &ValueFixed(val);
		str = xval->sval;
		ret.tm_year = 0;
		ret.tm_mon = 0;
		ret.tm_mday = 0;
		ret.tm_hour = StrToInt(str,2);	str += 2;
		ret.tm_min = StrToInt(str,2);	str += 2;
		ret.tm_sec = StrToInt(str,2);	str += 2;
		break;
	  case	GL_TYPE_INT:
		wt = (time_t)ValueInteger(val);
		localtime_r(&wt,&ret);
		ret.tm_year = 0;
		ret.tm_mon = 0;
		ret.tm_mday = 0;
		break;
	  case	GL_TYPE_FLOAT:
		wt = (time_t)ValueFloat(val);
		localtime_r(&wt,&ret);
		ret.tm_year = 0;
		ret.tm_mon = 0;
		ret.tm_mday = 0;
		break;
	  case	GL_TYPE_TIMESTAMP:
	  case	GL_TYPE_TIME:
		memcpy(&ret,&ValueDateTime(val),sizeof(struct tm));
		ret.tm_year = 0;
		ret.tm_mon = 0;
		ret.tm_mday = 0;
		break;
	  default:
		break;
	}
	return	(ret);
}
/**/

