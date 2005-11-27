/* TA-LIB Copyright (c) 1999-2005, Mario Fortier
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 *
 * - Neither name of author nor the names of its contributors
 *   may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* List of contributors:
 *
 *  Initial  Name/description
 *  -------------------------------------------------------------------
 *  MF       Mario Fortier
 *
 *
 * Change history:
 *
 *  MMDDYY BY   Description
 *  -------------------------------------------------------------------
 *  112400 MF   Template creation.
 *  052603 MF   Adapt code to compile with .NET Managed C++
 *  062804 MF   Resolve div by zero bug on limit case.
 */

/**** START GENCODE SECTION 1 - DO NOT DELETE THIS LINE ****/
/* All code within this section is automatically
 * generated by gen_code. Any modification will be lost
 * next time gen_code is run.
 */
/* Generated */ 
/* Generated */ #if defined( _MANAGED )
/* Generated */    #include "TA-Lib-Core.h"
/* Generated */    #define TA_INTERNAL_ERROR(Id) (NAMESPACE(TA_RetCode)TA_INTERNAL_ERROR)
/* Generated */    namespace TA { namespace Lib {
/* Generated */ #else
/* Generated */    #include <string.h>
/* Generated */    #include <math.h>
/* Generated */    #include "ta_func.h"
/* Generated */    #include "ta_trace.h"
/* Generated */ #endif
/* Generated */ 
/* Generated */ #ifndef TA_UTILITY_H
/* Generated */    #include "ta_utility.h"
/* Generated */ #endif
/* Generated */ 
/* Generated */ #ifndef TA_MEMORY_H
/* Generated */    #include "ta_memory.h"
/* Generated */ #endif
/* Generated */ 
/* Generated */ #define TA_PREFIX(x) TA_##x
/* Generated */ #define INPUT_TYPE   double
/* Generated */ 
/* Generated */ #if defined( _MANAGED )
/* Generated */ int Core::RSI_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
/* Generated */ 
/* Generated */ #else
/* Generated */ int TA_RSI_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
/* Generated */ 
/* Generated */ #endif
/**** END GENCODE SECTION 1 - DO NOT DELETE THIS LINE ****/
{
   /* insert lookback code here. */
   int retValue;

   retValue = optInTimePeriod + TA_Globals->unstablePeriod[(int)NAMESPACE(TA_FuncUnstId)TA_FUNC_UNST_RSI];
   if( TA_Globals->compatibility == NAMESPACE(TA_Compatibility)TA_COMPATIBILITY_METASTOCK )
      retValue--;

   return retValue;
}

/**** START GENCODE SECTION 2 - DO NOT DELETE THIS LINE ****/
/*
 * TA_RSI - Relative Strength Index
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
/* Generated */ 
/* Generated */ #if defined( _MANAGED )
/* Generated */ enum class Core::TA_RetCode Core::RSI( int    startIdx,
/* Generated */                                        int    endIdx,
/* Generated */                                        cli::array<double>^ inReal,
/* Generated */                                        int           optInTimePeriod, /* From 2 to 100000 */
/* Generated */                                        [Out]int%    outBegIdx,
/* Generated */                                        [Out]int%    outNbElement,
/* Generated */                                        cli::array<double>^  outReal )
/* Generated */ #else
/* Generated */ TA_RetCode TA_RSI( int    startIdx,
/* Generated */                    int    endIdx,
/* Generated */                    const double inReal[],
/* Generated */                    int           optInTimePeriod, /* From 2 to 100000 */
/* Generated */                    int          *outBegIdx,
/* Generated */                    int          *outNbElement,
/* Generated */                    double        outReal[] )
/* Generated */ #endif
/**** END GENCODE SECTION 2 - DO NOT DELETE THIS LINE ****/
{
   /* Insert local variables here. */
   int outIdx;

   int today, lookbackTotal, unstablePeriod, i;
   double prevGain, prevLoss, prevValue, savePrevValue;
   double tempValue1, tempValue2;

/**** START GENCODE SECTION 3 - DO NOT DELETE THIS LINE ****/
/* Generated */ 
/* Generated */ #ifndef TA_FUNC_NO_RANGE_CHECK
/* Generated */ 
/* Generated */    /* Validate the requested output range. */
/* Generated */    if( startIdx < 0 )
/* Generated */       return NAMESPACE(TA_RetCode)TA_OUT_OF_RANGE_START_INDEX;
/* Generated */    if( (endIdx < 0) || (endIdx < startIdx))
/* Generated */       return NAMESPACE(TA_RetCode)TA_OUT_OF_RANGE_END_INDEX;
/* Generated */ 
/* Generated */    /* Validate the parameters. */
/* Generated */    if( !inReal ) return NAMESPACE(TA_RetCode)TA_BAD_PARAM;
/* Generated */    /* min/max are checked for optInTimePeriod. */
/* Generated */    if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
/* Generated */       optInTimePeriod = 14;
/* Generated */    else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
/* Generated */       return NAMESPACE(TA_RetCode)TA_BAD_PARAM;
/* Generated */ 
/* Generated */    if( !outReal )
/* Generated */       return NAMESPACE(TA_RetCode)TA_BAD_PARAM;
/* Generated */ 
/* Generated */ #endif /* TA_FUNC_NO_RANGE_CHECK */
/* Generated */ 
/**** END GENCODE SECTION 3 - DO NOT DELETE THIS LINE ****/

   /* Insert TA function code here. */

   /* The following algorithm is base on the original 
    * work from Wilder's and shall represent the
    * original idea behind the classic RSI.
    *
    * Metastock is starting the calculation one price
    * bar earlier. To make this possible, they assume
    * that the very first bar will be identical to the
    * previous one (no gain or loss).
    */

   /* If changing this function, please check also CMO 
    * which is mostly identical (just different in one step 
    * of calculation).
    */

   VALUE_HANDLE_DEREF_TO_ZERO(outBegIdx);
   VALUE_HANDLE_DEREF_TO_ZERO(outNbElement);
   
   /* Adjust startIdx to account for the lookback period. */
   lookbackTotal = TA_RSI_Lookback( optInTimePeriod );

   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
      return NAMESPACE(TA_RetCode)TA_SUCCESS;

   outIdx = 0; /* Index into the output. */

   /* Trap special case where the period is '1'.
    * In that case, just copy the input into the
    * output for the requested range (as-is !)
    */
   if( optInTimePeriod == 1 )
   {
      VALUE_HANDLE_DEREF(outBegIdx) = startIdx;
      i = (endIdx-startIdx)+1;
      VALUE_HANDLE_DEREF(outNbElement) = i;
      ARRAY_MEMMOVE( outReal, 0, inReal, startIdx, i );
      return NAMESPACE(TA_RetCode)TA_SUCCESS;
   }

   /* Accumulate Wilder's "Average Gain" and "Average Loss" 
    * among the initial period.
    */
   today = startIdx-lookbackTotal;
   prevValue = inReal[today];

   unstablePeriod = TA_Globals->unstablePeriod[(int)NAMESPACE(TA_FuncUnstId)TA_FUNC_UNST_RSI];

   /* If there is no unstable period,
    * calculate the 'additional' initial
    * price bar who is particuliar to
    * metastock.
    * If there is an unstable period,
    * no need to calculate since this
    * first value will be surely skip.
    */
   if( (unstablePeriod == 0) && 
       (TA_Globals->compatibility == NAMESPACE(TA_Compatibility)TA_COMPATIBILITY_METASTOCK))
   {
      /* Preserve prevValue because it may get 
       * overwritten by the output.
       *(because output ptr could be the same as input ptr).
       */
      savePrevValue = prevValue;

      /* No unstable period, so must calculate first output
       * particular to Metastock.
       * (Metastock re-use the first price bar, so there
       *  is no loss/gain at first. Beats me why they
       *  are doing all this).
       */
      prevGain = 0.0;
      prevLoss = 0.0;
      for( i=optInTimePeriod; i > 0; i-- )
      {
         tempValue1 = inReal[today++];
         tempValue2 = tempValue1 - prevValue;
         prevValue  = tempValue1;
         if( tempValue2 < 0 )
            prevLoss -= tempValue2;
         else
            prevGain += tempValue2;
      }


      tempValue1 = prevLoss/optInTimePeriod;
      tempValue2 = prevGain/optInTimePeriod;

      /* Write the output. */
      tempValue1 = tempValue2+tempValue1;
      if( !TA_IS_ZERO(tempValue1) )
         outReal[outIdx++] = 100*(tempValue2/tempValue1);
      else
         outReal[outIdx++] = 0.0;

      /* Are we done? */
      if( today > endIdx )
      {
         VALUE_HANDLE_DEREF(outBegIdx)    = startIdx;
         VALUE_HANDLE_DEREF(outNbElement) = outIdx;
         return NAMESPACE(TA_RetCode)TA_SUCCESS;
      }

      /* Start over for the next price bar. */
      today -= optInTimePeriod;
      prevValue = savePrevValue;
   }


   /* Remaining of the processing is identical
    * for both Classic calculation and Metastock.
    */
   prevGain = 0.0;
   prevLoss = 0.0;
   today++;
   for( i=optInTimePeriod; i > 0; i-- )
   {
      tempValue1 = inReal[today++];
      tempValue2 = tempValue1 - prevValue;
      prevValue  = tempValue1;
      if( tempValue2 < 0 )
         prevLoss -= tempValue2;
      else
         prevGain += tempValue2;
   }

   
   /* Subsequent prevLoss and prevGain are smoothed
    * using the previous values (Wilder's approach).
    *  1) Multiply the previous by 'period-1'. 
    *  2) Add today value.
    *  3) Divide by 'period'.
    */
   prevLoss /= optInTimePeriod;
   prevGain /= optInTimePeriod;

   /* Often documentation present the RSI calculation as follow:
    *    RSI = 100 - (100 / 1 + (prevGain/prevLoss))
    *
    * The following is equivalent:
    *    RSI = 100 * (prevGain/(prevGain+prevLoss))
    *
    * The second equation is used here for speed optimization.
    */
   if( today > startIdx )
   {
      tempValue1 = prevGain+prevLoss;
      if( !TA_IS_ZERO(tempValue1) )
         outReal[outIdx++] = 100.0*(prevGain/tempValue1);
      else
         outReal[outIdx++] = 0.0;
   }
   else
   {
      /* Skip the unstable period. Do the processing 
       * but do not write it in the output.
       */   
      while( today < startIdx )
      {
         tempValue1 = inReal[today];
         tempValue2 = tempValue1 - prevValue;
         prevValue  = tempValue1;

         prevLoss *= (optInTimePeriod-1);
         prevGain *= (optInTimePeriod-1);
         if( tempValue2 < 0 )
            prevLoss -= tempValue2;
         else
            prevGain += tempValue2;

         prevLoss /= optInTimePeriod;
         prevGain /= optInTimePeriod;

         today++;
      }
   }

   /* Unstable period skipped... now continue
    * processing if needed.
    */
   while( today <= endIdx )
   {
      tempValue1 = inReal[today++];
      tempValue2 = tempValue1 - prevValue;
      prevValue  = tempValue1;

      prevLoss *= (optInTimePeriod-1);
      prevGain *= (optInTimePeriod-1);
      if( tempValue2 < 0 )
         prevLoss -= tempValue2;
      else
         prevGain += tempValue2;

      prevLoss /= optInTimePeriod;
      prevGain /= optInTimePeriod;
      tempValue1 = prevGain+prevLoss;
      if( !TA_IS_ZERO(tempValue1) )
         outReal[outIdx++] = 100.0*(prevGain/tempValue1);
      else
         outReal[outIdx++] = 0.0;
   }

   VALUE_HANDLE_DEREF(outBegIdx) = startIdx;
   VALUE_HANDLE_DEREF(outNbElement) = outIdx;

   return NAMESPACE(TA_RetCode)TA_SUCCESS;
}


/**** START GENCODE SECTION 4 - DO NOT DELETE THIS LINE ****/
/* Generated */ 
/* Generated */ #define  USE_SINGLE_PRECISION_INPUT
/* Generated */ #if !defined( _MANAGED )
/* Generated */    #undef   TA_PREFIX
/* Generated */    #define  TA_PREFIX(x) TA_S_##x
/* Generated */ #endif
/* Generated */ #undef   INPUT_TYPE
/* Generated */ #define  INPUT_TYPE float
/* Generated */ #if defined( _MANAGED )
/* Generated */ enum class Core::TA_RetCode Core::RSI( int    startIdx,
/* Generated */                                        int    endIdx,
/* Generated */                                        cli::array<float>^ inReal,
/* Generated */                                        int           optInTimePeriod, /* From 2 to 100000 */
/* Generated */                                        [Out]int%    outBegIdx,
/* Generated */                                        [Out]int%    outNbElement,
/* Generated */                                        cli::array<double>^  outReal )
/* Generated */ #else
/* Generated */ TA_RetCode TA_S_RSI( int    startIdx,
/* Generated */                      int    endIdx,
/* Generated */                      const float  inReal[],
/* Generated */                      int           optInTimePeriod, /* From 2 to 100000 */
/* Generated */                      int          *outBegIdx,
/* Generated */                      int          *outNbElement,
/* Generated */                      double        outReal[] )
/* Generated */ #endif
/* Generated */ {
/* Generated */    int outIdx;
/* Generated */    int today, lookbackTotal, unstablePeriod, i;
/* Generated */    double prevGain, prevLoss, prevValue, savePrevValue;
/* Generated */    double tempValue1, tempValue2;
/* Generated */  #ifndef TA_FUNC_NO_RANGE_CHECK
/* Generated */     if( startIdx < 0 )
/* Generated */        return NAMESPACE(TA_RetCode)TA_OUT_OF_RANGE_START_INDEX;
/* Generated */     if( (endIdx < 0) || (endIdx < startIdx))
/* Generated */        return NAMESPACE(TA_RetCode)TA_OUT_OF_RANGE_END_INDEX;
/* Generated */     if( !inReal ) return NAMESPACE(TA_RetCode)TA_BAD_PARAM;
/* Generated */     if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
/* Generated */        optInTimePeriod = 14;
/* Generated */     else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
/* Generated */        return NAMESPACE(TA_RetCode)TA_BAD_PARAM;
/* Generated */     if( !outReal )
/* Generated */        return NAMESPACE(TA_RetCode)TA_BAD_PARAM;
/* Generated */  #endif 
/* Generated */    VALUE_HANDLE_DEREF_TO_ZERO(outBegIdx);
/* Generated */    VALUE_HANDLE_DEREF_TO_ZERO(outNbElement);
/* Generated */    lookbackTotal = TA_RSI_Lookback( optInTimePeriod );
/* Generated */    if( startIdx < lookbackTotal )
/* Generated */       startIdx = lookbackTotal;
/* Generated */    if( startIdx > endIdx )
/* Generated */       return NAMESPACE(TA_RetCode)TA_SUCCESS;
/* Generated */    outIdx = 0; 
/* Generated */    if( optInTimePeriod == 1 )
/* Generated */    {
/* Generated */       VALUE_HANDLE_DEREF(outBegIdx) = startIdx;
/* Generated */       i = (endIdx-startIdx)+1;
/* Generated */       VALUE_HANDLE_DEREF(outNbElement) = i;
/* Generated */       ARRAY_MEMMOVE( outReal, 0, inReal, startIdx, i );
/* Generated */       return NAMESPACE(TA_RetCode)TA_SUCCESS;
/* Generated */    }
/* Generated */    today = startIdx-lookbackTotal;
/* Generated */    prevValue = inReal[today];
/* Generated */    unstablePeriod = TA_Globals->unstablePeriod[(int)NAMESPACE(TA_FuncUnstId)TA_FUNC_UNST_RSI];
/* Generated */    if( (unstablePeriod == 0) && 
/* Generated */        (TA_Globals->compatibility == NAMESPACE(TA_Compatibility)TA_COMPATIBILITY_METASTOCK))
/* Generated */    {
/* Generated */       savePrevValue = prevValue;
/* Generated */       prevGain = 0.0;
/* Generated */       prevLoss = 0.0;
/* Generated */       for( i=optInTimePeriod; i > 0; i-- )
/* Generated */       {
/* Generated */          tempValue1 = inReal[today++];
/* Generated */          tempValue2 = tempValue1 - prevValue;
/* Generated */          prevValue  = tempValue1;
/* Generated */          if( tempValue2 < 0 )
/* Generated */             prevLoss -= tempValue2;
/* Generated */          else
/* Generated */             prevGain += tempValue2;
/* Generated */       }
/* Generated */       tempValue1 = prevLoss/optInTimePeriod;
/* Generated */       tempValue2 = prevGain/optInTimePeriod;
/* Generated */       tempValue1 = tempValue2+tempValue1;
/* Generated */       if( !TA_IS_ZERO(tempValue1) )
/* Generated */          outReal[outIdx++] = 100*(tempValue2/tempValue1);
/* Generated */       else
/* Generated */          outReal[outIdx++] = 0.0;
/* Generated */       if( today > endIdx )
/* Generated */       {
/* Generated */          VALUE_HANDLE_DEREF(outBegIdx)    = startIdx;
/* Generated */          VALUE_HANDLE_DEREF(outNbElement) = outIdx;
/* Generated */          return NAMESPACE(TA_RetCode)TA_SUCCESS;
/* Generated */       }
/* Generated */       today -= optInTimePeriod;
/* Generated */       prevValue = savePrevValue;
/* Generated */    }
/* Generated */    prevGain = 0.0;
/* Generated */    prevLoss = 0.0;
/* Generated */    today++;
/* Generated */    for( i=optInTimePeriod; i > 0; i-- )
/* Generated */    {
/* Generated */       tempValue1 = inReal[today++];
/* Generated */       tempValue2 = tempValue1 - prevValue;
/* Generated */       prevValue  = tempValue1;
/* Generated */       if( tempValue2 < 0 )
/* Generated */          prevLoss -= tempValue2;
/* Generated */       else
/* Generated */          prevGain += tempValue2;
/* Generated */    }
/* Generated */    prevLoss /= optInTimePeriod;
/* Generated */    prevGain /= optInTimePeriod;
/* Generated */    if( today > startIdx )
/* Generated */    {
/* Generated */       tempValue1 = prevGain+prevLoss;
/* Generated */       if( !TA_IS_ZERO(tempValue1) )
/* Generated */          outReal[outIdx++] = 100.0*(prevGain/tempValue1);
/* Generated */       else
/* Generated */          outReal[outIdx++] = 0.0;
/* Generated */    }
/* Generated */    else
/* Generated */    {
/* Generated */       while( today < startIdx )
/* Generated */       {
/* Generated */          tempValue1 = inReal[today];
/* Generated */          tempValue2 = tempValue1 - prevValue;
/* Generated */          prevValue  = tempValue1;
/* Generated */          prevLoss *= (optInTimePeriod-1);
/* Generated */          prevGain *= (optInTimePeriod-1);
/* Generated */          if( tempValue2 < 0 )
/* Generated */             prevLoss -= tempValue2;
/* Generated */          else
/* Generated */             prevGain += tempValue2;
/* Generated */          prevLoss /= optInTimePeriod;
/* Generated */          prevGain /= optInTimePeriod;
/* Generated */          today++;
/* Generated */       }
/* Generated */    }
/* Generated */    while( today <= endIdx )
/* Generated */    {
/* Generated */       tempValue1 = inReal[today++];
/* Generated */       tempValue2 = tempValue1 - prevValue;
/* Generated */       prevValue  = tempValue1;
/* Generated */       prevLoss *= (optInTimePeriod-1);
/* Generated */       prevGain *= (optInTimePeriod-1);
/* Generated */       if( tempValue2 < 0 )
/* Generated */          prevLoss -= tempValue2;
/* Generated */       else
/* Generated */          prevGain += tempValue2;
/* Generated */       prevLoss /= optInTimePeriod;
/* Generated */       prevGain /= optInTimePeriod;
/* Generated */       tempValue1 = prevGain+prevLoss;
/* Generated */       if( !TA_IS_ZERO(tempValue1) )
/* Generated */          outReal[outIdx++] = 100.0*(prevGain/tempValue1);
/* Generated */       else
/* Generated */          outReal[outIdx++] = 0.0;
/* Generated */    }
/* Generated */    VALUE_HANDLE_DEREF(outBegIdx) = startIdx;
/* Generated */    VALUE_HANDLE_DEREF(outNbElement) = outIdx;
/* Generated */    return NAMESPACE(TA_RetCode)TA_SUCCESS;
/* Generated */ }
/* Generated */ 
/* Generated */ #if defined( _MANAGED )
/* Generated */ }} // Close namespace TA.Lib
/* Generated */ #endif
/**** END GENCODE SECTION 4 - DO NOT DELETE THIS LINE ****/

