/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef _XMLOFF_PROPERTYACTIONSOASIS_HXX
#define _XMLOFF_PROPERTYACTIONSOASIS_HXX

#include "TransformerAction.hxx"
#include "TransformerActionInit.hxx"
#include "AttrTransformerAction.hxx"

enum XMLPropOASISTransformerAction
{
	XML_OPTACTION_LINE_MODE=XML_ATACTION_USER_DEFINED,
	XML_OPTACTION_UNDERLINE_TYPE,
	XML_OPTACTION_UNDERLINE_STYLE,
	XML_OPTACTION_UNDERLINE_WIDTH,
	XML_OPTACTION_LINETHROUGH_TYPE,
	XML_OPTACTION_LINETHROUGH_STYLE,
	XML_OPTACTION_LINETHROUGH_WIDTH,
	XML_OPTACTION_LINETHROUGH_TEXT,
	XML_OPTACTION_KEEP_WITH_NEXT,
	XML_OPTACTION_INTERPOLATION,
	XML_OPTACTION_INTERVAL_MAJOR,
	XML_OPTACTION_INTERVAL_MINOR_DIVISOR,
    XML_OPTACTION_SYMBOL_TYPE,
    XML_OPTACTION_SYMBOL_NAME,
	XML_OPTACTION_OPACITY,
	XML_OPTACTION_IMAGE_OPACITY,
    XML_OPTACTION_KEEP_TOGETHER,
	XML_OPTACTION_CONTROL_TEXT_ALIGN,
	XML_OPTACTION_DRAW_WRITING_MODE,
	XML_ATACTION_CAPTION_ESCAPE_OASIS,
	XML_ATACTION_DECODE_PROTECT,
	XML_OPTACTION_END=XML_ATACTION_END
};

extern XMLTransformerActionInit aGraphicPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aDrawingPagePropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aPageLayoutPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aHeaderFooterPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aTextPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aParagraphPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aSectionPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aTablePropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aTableColumnPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aTableRowPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aTableCellPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aListLevelPropertyOASISAttrActionTable[];
extern XMLTransformerActionInit aChartPropertyOASISAttrActionTable[];

#endif	//  _XMLOFF_PROPERTYACTIONSOASIS_HXX
