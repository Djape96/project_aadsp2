################################################################################
##*/**
##** @file ModuleName.h
##**
##** Module:
##** Description:
##** $Source: $
##** $Revision: $
##** $Date: $
##** $Author: $
##**
##**/
################################################################################
  .list -cond
  .if !defined(_ModuleName_h_)
_ModuleName_h_


#	Public/Extern Macro:
#-----------------------------------------------
	.if defined(_ModuleName_a_)
	   .macro
		_ModuleName_import_export:		%var
		.public %var
		.endm
	.else
		.macro
		_ModuleName_import_export:		%var
		.extern %var
		.endm
	.endif
#-----------------------------------------------

#	INCLUDES:
################################################################################
	.if defined(_ModuleName_a_)
		#TODO: Insert aditional includes here
#		.include "FileName.h"
	.endif
################################################################################


#	PUBLIC/EXTERN:
################################################################################

		#TODO: Type public/extern definitions here
#		_ModuleName_import_export:		function_or_variable_name

################################################################################
  .endif


