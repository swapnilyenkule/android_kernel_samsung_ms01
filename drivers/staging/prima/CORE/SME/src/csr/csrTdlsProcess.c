/*
<<<<<<< HEAD
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2012-2013 The Linux Foundation. All rights reserved.
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
<<<<<<< HEAD
/*
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
=======

/*
 * This file was originally distributed by Qualcomm Atheros, Inc.
 * under proprietary terms before Copyright ownership was assigned
 * to the Linux Foundation.
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */

/** ------------------------------------------------------------------------- * 
    ------------------------------------------------------------------------- *  

  
    \file csrTdlsProcess.c
  
    Implementation for the TDLS interface to PE.
  
<<<<<<< HEAD
   Copyright (c) 2013 Qualcomm Atheros, Inc.All Rights Reserved.
   Qualcomm Atheros Confidential and Proprietary.
    
   Copyright (c) 2010 Qualcomm Technologies, Inc.All Rights Reserved.
   Qualcomm Technologies Confidential and Proprietary

=======
    Copyright (C) 2010 Qualcomm, Incorporated
  
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 
   ========================================================================== */

#ifdef FEATURE_WLAN_TDLS

#include "aniGlobal.h" //for tpAniSirGlobal
#include "palApi.h"
#include "csrInsideApi.h"
#include "smeInside.h"
#include "smsDebug.h"

#include "csrSupport.h"
#include "wlan_qct_tl.h"

#include "vos_diag_core_log.h"
#include "vos_diag_core_event.h"
#include "csrInternal.h"



#ifdef FEATURE_WLAN_TDLS_INTERNAL
/*
 * initialize Discovery list 
 */
eHalStatus csrTdlsInitPeerList(tpAniSirGlobal pMac )
{
    tCsrTdlsCtxStruct *tdlsDisInfo = &pMac->tdlsCtx ;
    csrLLOpen(pMac->hHdd, &tdlsDisInfo->tdlsPotentialPeerList) ;

    return eHAL_STATUS_SUCCESS ;
}
#endif

/*
 * common routine to remove TDLS cmd from SME command list..
 * commands are removed after getting reponse from PE.
 */
eHalStatus csrTdlsRemoveSmeCmd(tpAniSirGlobal pMac, eSmeCommandType cmdType)
{
    eHalStatus status = eHAL_STATUS_FAILURE;
    tListElem *pEntry;
    tSmeCmd *pCommand;

    pEntry = csrLLPeekHead(&pMac->sme.smeCmdActiveList, LL_ACCESS_LOCK);
    if( pEntry )
    {
        pCommand = GET_BASE_ADDR(pEntry, tSmeCmd, Link);
        if( cmdType == pCommand->command )
        {
            if( csrLLRemoveEntry( &pMac->sme.smeCmdActiveList, 
                                             pEntry, LL_ACCESS_LOCK ) )
            {
                vos_mem_zero( &pCommand->u.tdlsCmd, sizeof( tTdlsCmd ) );
                csrReleaseCommand( pMac, pCommand );
                smeProcessPendingQueue( pMac );
                status = eHAL_STATUS_SUCCESS ;
            }
        }
    }
    return status ;
}
    
/*
 * TDLS request API, called from HDD to send a TDLS frame 
 * in SME/CSR and send message to PE to trigger TDLS discovery procedure.
 */
eHalStatus csrTdlsSendMgmtReq(tHalHandle hHal, tANI_U8 sessionId, tCsrTdlsSendMgmt *tdlsSendMgmt)
{
    tpAniSirGlobal pMac = PMAC_STRUCT( hHal );
    tSmeCmd *tdlsSendMgmtCmd ;
    eHalStatus status = eHAL_STATUS_FAILURE ;
<<<<<<< HEAD
 
=======

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
    //If connected and in Infra. Only then allow this
    if( CSR_IS_SESSION_VALID( pMac, sessionId ) && 
        csrIsConnStateConnectedInfra( pMac, sessionId ) &&
        (NULL != tdlsSendMgmt) )
    {
        tdlsSendMgmtCmd = csrGetCommandBuffer(pMac) ;

        if(tdlsSendMgmtCmd)
        {
            tTdlsSendMgmtCmdInfo *tdlsSendMgmtCmdInfo = 
                            &tdlsSendMgmtCmd->u.tdlsCmd.u.tdlsSendMgmtCmdInfo ;

<<<<<<< HEAD
=======
            vos_mem_zero(&tdlsSendMgmtCmd->u.tdlsCmd, sizeof(tTdlsCmd));

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
            tdlsSendMgmtCmd->sessionId = sessionId;

            tdlsSendMgmtCmdInfo->frameType = tdlsSendMgmt->frameType ;   
            tdlsSendMgmtCmdInfo->dialog = tdlsSendMgmt->dialog ;   
            tdlsSendMgmtCmdInfo->statusCode = tdlsSendMgmt->statusCode ;
            tdlsSendMgmtCmdInfo->responder = tdlsSendMgmt->responder;
<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, tdlsSendMgmtCmdInfo->peerMac, 
=======
            tdlsSendMgmtCmdInfo->peerCapability = tdlsSendMgmt->peerCapability;
            vos_mem_copy(tdlsSendMgmtCmdInfo->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                   tdlsSendMgmt->peerMac, sizeof(tSirMacAddr)) ; 

            if( (0 != tdlsSendMgmt->len) && (NULL != tdlsSendMgmt->buf) )
            {
<<<<<<< HEAD
                status = palAllocateMemory( pMac->hHdd, (void **)&tdlsSendMgmtCmdInfo->buf, 
                        tdlsSendMgmt->len );
=======
                tdlsSendMgmtCmdInfo->buf = vos_mem_malloc(tdlsSendMgmt->len);
                if ( NULL == tdlsSendMgmtCmdInfo->buf )
                    status = eHAL_STATUS_FAILURE;
                else
                    status = eHAL_STATUS_SUCCESS;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                if(!HAL_STATUS_SUCCESS( status ) )
                {
                    smsLog( pMac, LOGE, FL("Alloc Failed") );
                    VOS_ASSERT(0) ;
                    return status ;
                }
<<<<<<< HEAD
                palCopyMemory(pMac->hHdd, tdlsSendMgmtCmdInfo->buf,
=======
                vos_mem_copy(tdlsSendMgmtCmdInfo->buf,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                        tdlsSendMgmt->buf, tdlsSendMgmt->len );
                tdlsSendMgmtCmdInfo->len = tdlsSendMgmt->len;
            }
            else
            {
                tdlsSendMgmtCmdInfo->buf = NULL;
                tdlsSendMgmtCmdInfo->len = 0;
            }

            tdlsSendMgmtCmd->command = eSmeCommandTdlsSendMgmt ;
            tdlsSendMgmtCmd->u.tdlsCmd.size = sizeof(tTdlsSendMgmtCmdInfo) ;
            smePushCommand(pMac, tdlsSendMgmtCmd, FALSE) ;
            status = eHAL_STATUS_SUCCESS ;
        }
    }

    return status ;
}

/*
<<<<<<< HEAD
 * TDLS request API, called from HDD to add a TDLS peer 
=======
 * TDLS request API, called from HDD to modify an existing TDLS peer
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
 */
eHalStatus csrTdlsChangePeerSta(tHalHandle hHal, tANI_U8 sessionId, tSirMacAddr peerMac,
                                tCsrStaParams *pstaParams)
{
    tpAniSirGlobal pMac = PMAC_STRUCT( hHal );
    tSmeCmd *tdlsAddStaCmd ;
    eHalStatus status = eHAL_STATUS_FAILURE ;

<<<<<<< HEAD
=======
    if (NULL == pstaParams)
        return status;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
    //If connected and in Infra. Only then allow this
    if (CSR_IS_SESSION_VALID( pMac, sessionId ) &&
        csrIsConnStateConnectedInfra( pMac, sessionId ) &&
        (NULL != peerMac)){

        tdlsAddStaCmd = csrGetCommandBuffer(pMac) ;

        if (tdlsAddStaCmd)
        {
            tTdlsAddStaCmdInfo *tdlsAddStaCmdInfo =
                         &tdlsAddStaCmd->u.tdlsCmd.u.tdlsAddStaCmdInfo ;

<<<<<<< HEAD
=======
            vos_mem_zero(&tdlsAddStaCmd->u.tdlsCmd, sizeof(tTdlsCmd));

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
            tdlsAddStaCmdInfo->tdlsAddOper = TDLS_OPER_UPDATE;

            tdlsAddStaCmd->sessionId = sessionId;

<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, tdlsAddStaCmdInfo->peerMac,
=======
            vos_mem_copy(tdlsAddStaCmdInfo->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                          peerMac, sizeof(tSirMacAddr)) ;
            tdlsAddStaCmdInfo->capability = pstaParams->capability;
            tdlsAddStaCmdInfo->uapsdQueues = pstaParams->uapsd_queues;
            tdlsAddStaCmdInfo->maxSp = pstaParams->max_sp;
<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, tdlsAddStaCmdInfo->extnCapability,
=======
            vos_mem_copy(tdlsAddStaCmdInfo->extnCapability,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                          pstaParams->extn_capability,
                          sizeof(pstaParams->extn_capability));

            tdlsAddStaCmdInfo->htcap_present = pstaParams->htcap_present;
            if(pstaParams->htcap_present)
<<<<<<< HEAD
                palCopyMemory(pMac->hHdd, &tdlsAddStaCmdInfo->HTCap,
                              &pstaParams->HTCap, sizeof(pstaParams->HTCap));
            else
                palZeroMemory(pMac->hHdd, &tdlsAddStaCmdInfo->HTCap, sizeof(pstaParams->HTCap));

            tdlsAddStaCmdInfo->vhtcap_present = pstaParams->vhtcap_present;
            if(pstaParams->vhtcap_present)
                palCopyMemory(pMac->hHdd, &tdlsAddStaCmdInfo->VHTCap,
                              &pstaParams->VHTCap, sizeof(pstaParams->VHTCap));
            else
                palZeroMemory(pMac->hHdd, &tdlsAddStaCmdInfo->VHTCap, sizeof(pstaParams->VHTCap));

			tdlsAddStaCmdInfo->supportedRatesLen = pstaParams->supported_rates_len;

            if (0 != pstaParams->supported_rates_len)
                palCopyMemory(pMac->hHdd, &tdlsAddStaCmdInfo->supportedRates,
=======
                vos_mem_copy( &tdlsAddStaCmdInfo->HTCap,
                              &pstaParams->HTCap, sizeof(pstaParams->HTCap));
            else
                vos_mem_set(&tdlsAddStaCmdInfo->HTCap, sizeof(pstaParams->HTCap), 0);

            tdlsAddStaCmdInfo->vhtcap_present = pstaParams->vhtcap_present;
            if(pstaParams->vhtcap_present)
                vos_mem_copy( &tdlsAddStaCmdInfo->VHTCap,
                              &pstaParams->VHTCap, sizeof(pstaParams->VHTCap));
            else
                vos_mem_set(&tdlsAddStaCmdInfo->VHTCap, sizeof(pstaParams->VHTCap), 0);

            tdlsAddStaCmdInfo->supportedRatesLen = pstaParams->supported_rates_len;

            if (0 != pstaParams->supported_rates_len)
                vos_mem_copy( &tdlsAddStaCmdInfo->supportedRates,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                              pstaParams->supported_rates,
                              pstaParams->supported_rates_len);

            tdlsAddStaCmd->command = eSmeCommandTdlsAddPeer;
            tdlsAddStaCmd->u.tdlsCmd.size = sizeof(tTdlsAddStaCmdInfo) ;
            smePushCommand(pMac, tdlsAddStaCmd, FALSE) ;
            status = eHAL_STATUS_SUCCESS ;
        }
    }

    return status ;
}
/*
 * TDLS request API, called from HDD to Send Link Establishment Parameters
 */
VOS_STATUS csrTdlsSendLinkEstablishParams(tHalHandle hHal,
                                                 tANI_U8 sessionId,
                                                 tSirMacAddr peerMac,
                                                 tCsrTdlsLinkEstablishParams *tdlsLinkEstablishParams)
{
    tpAniSirGlobal pMac = PMAC_STRUCT( hHal );
    tSmeCmd *tdlsLinkEstablishCmd;
    eHalStatus status = eHAL_STATUS_FAILURE ;
    //If connected and in Infra. Only then allow this
    if( CSR_IS_SESSION_VALID( pMac, sessionId ) &&
        csrIsConnStateConnectedInfra( pMac, sessionId ) &&
        (NULL != peerMac) )
    {
        tdlsLinkEstablishCmd = csrGetCommandBuffer(pMac) ;

        if(tdlsLinkEstablishCmd)
        {
            tTdlsLinkEstablishCmdInfo *tdlsLinkEstablishCmdInfo =
            &tdlsLinkEstablishCmd->u.tdlsCmd.u.tdlsLinkEstablishCmdInfo ;

<<<<<<< HEAD
            tdlsLinkEstablishCmd->sessionId = sessionId;

            palCopyMemory(pMac->hHdd, tdlsLinkEstablishCmdInfo->peerMac,
                          peerMac, sizeof(tSirMacAddr));
            tdlsLinkEstablishCmdInfo->isBufSta = tdlsLinkEstablishParams->isBufSta;
=======
            vos_mem_zero(&tdlsLinkEstablishCmd->u.tdlsCmd, sizeof(tTdlsCmd));

            tdlsLinkEstablishCmd->sessionId = sessionId;

            vos_mem_copy( tdlsLinkEstablishCmdInfo->peerMac,
                          peerMac, sizeof(tSirMacAddr));
            tdlsLinkEstablishCmdInfo->isBufSta = tdlsLinkEstablishParams->isBufSta;
            tdlsLinkEstablishCmdInfo->isResponder = tdlsLinkEstablishParams->isResponder;
            tdlsLinkEstablishCmdInfo->maxSp = tdlsLinkEstablishParams->maxSp;
            tdlsLinkEstablishCmdInfo->uapsdQueues = tdlsLinkEstablishParams->uapsdQueues;
            tdlsLinkEstablishCmdInfo->isOffChannelSupported =
                                               tdlsLinkEstablishParams->isOffChannelSupported;

            vos_mem_copy(tdlsLinkEstablishCmdInfo->supportedChannels,
                          tdlsLinkEstablishParams->supportedChannels,
                          tdlsLinkEstablishParams->supportedChannelsLen);
            tdlsLinkEstablishCmdInfo->supportedChannelsLen =
                                    tdlsLinkEstablishParams->supportedChannelsLen;

            vos_mem_copy(tdlsLinkEstablishCmdInfo->supportedOperClasses,
                          tdlsLinkEstablishParams->supportedOperClasses,
                          tdlsLinkEstablishParams->supportedOperClassesLen);
            tdlsLinkEstablishCmdInfo->supportedOperClassesLen =
                                    tdlsLinkEstablishParams->supportedOperClassesLen;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
            tdlsLinkEstablishCmdInfo->isResponder= tdlsLinkEstablishParams->isResponder;
            tdlsLinkEstablishCmdInfo->maxSp= tdlsLinkEstablishParams->maxSp;
            tdlsLinkEstablishCmdInfo->uapsdQueues= tdlsLinkEstablishParams->uapsdQueues;
            tdlsLinkEstablishCmd->command = eSmeCommandTdlsLinkEstablish ;
            tdlsLinkEstablishCmd->u.tdlsCmd.size = sizeof(tTdlsLinkEstablishCmdInfo) ;
            smePushCommand(pMac, tdlsLinkEstablishCmd, FALSE) ;
            status = eHAL_STATUS_SUCCESS ;
        }
    }

    return status ;
}

/*
 * TDLS request API, called from HDD to add a TDLS peer
 */
eHalStatus csrTdlsAddPeerSta(tHalHandle hHal, tANI_U8 sessionId, tSirMacAddr peerMac)
{
    tpAniSirGlobal pMac = PMAC_STRUCT( hHal );
    tSmeCmd *tdlsAddStaCmd ;
    eHalStatus status = eHAL_STATUS_FAILURE ;
 
    //If connected and in Infra. Only then allow this
    if( CSR_IS_SESSION_VALID( pMac, sessionId ) && 
        csrIsConnStateConnectedInfra( pMac, sessionId ) &&
        (NULL != peerMac) )
    {
        tdlsAddStaCmd = csrGetCommandBuffer(pMac) ;

        if(tdlsAddStaCmd)
        {
            tTdlsAddStaCmdInfo *tdlsAddStaCmdInfo = 
                &tdlsAddStaCmd->u.tdlsCmd.u.tdlsAddStaCmdInfo ;

<<<<<<< HEAD
            tdlsAddStaCmd->sessionId = sessionId;
            tdlsAddStaCmdInfo->tdlsAddOper = TDLS_OPER_ADD;

            palCopyMemory(pMac->hHdd, tdlsAddStaCmdInfo->peerMac, 
=======
            vos_mem_zero(&tdlsAddStaCmd->u.tdlsCmd, sizeof(tTdlsCmd));

            tdlsAddStaCmd->sessionId = sessionId;
            tdlsAddStaCmdInfo->tdlsAddOper = TDLS_OPER_ADD;

            vos_mem_copy( tdlsAddStaCmdInfo->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                    peerMac, sizeof(tSirMacAddr)) ; 

            tdlsAddStaCmd->command = eSmeCommandTdlsAddPeer ;
            tdlsAddStaCmd->u.tdlsCmd.size = sizeof(tTdlsAddStaCmdInfo) ;
            smePushCommand(pMac, tdlsAddStaCmd, FALSE) ;
            status = eHAL_STATUS_SUCCESS ;
        }
    }

    return status ;
}

/*
 * TDLS request API, called from HDD to delete a TDLS peer
 */
eHalStatus csrTdlsDelPeerSta(tHalHandle hHal, tANI_U8 sessionId, tSirMacAddr peerMac)
{
    tpAniSirGlobal pMac = PMAC_STRUCT( hHal );
    tSmeCmd *tdlsDelStaCmd ;
    eHalStatus status = eHAL_STATUS_FAILURE ;
 
    //If connected and in Infra. Only then allow this
    if( CSR_IS_SESSION_VALID( pMac, sessionId ) && 
        csrIsConnStateConnectedInfra( pMac, sessionId ) &&
        (NULL != peerMac) )
    {
        tdlsDelStaCmd = csrGetCommandBuffer(pMac) ;

        if(tdlsDelStaCmd)
        {
            tTdlsDelStaCmdInfo *tdlsDelStaCmdInfo = 
                            &tdlsDelStaCmd->u.tdlsCmd.u.tdlsDelStaCmdInfo ;

<<<<<<< HEAD
            tdlsDelStaCmd->sessionId = sessionId;

            palCopyMemory(pMac->hHdd, tdlsDelStaCmdInfo->peerMac, 
=======
            vos_mem_zero(&tdlsDelStaCmd->u.tdlsCmd, sizeof(tTdlsCmd));

            tdlsDelStaCmd->sessionId = sessionId;

            vos_mem_copy(tdlsDelStaCmdInfo->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                   peerMac, sizeof(tSirMacAddr)) ; 

            tdlsDelStaCmd->command = eSmeCommandTdlsDelPeer ;
            tdlsDelStaCmd->u.tdlsCmd.size = sizeof(tTdlsDelStaCmdInfo) ;
            smePushCommand(pMac, tdlsDelStaCmd, FALSE) ;
            status = eHAL_STATUS_SUCCESS ;
        }
    }

    return status ;
}
<<<<<<< HEAD
=======

//tdlsoffchan
/*
 * TDLS request API, called from HDD to Send Channel Switch Parameters
 */
VOS_STATUS csrTdlsSendChanSwitchReq(tHalHandle hHal,
                                    tANI_U8 sessionId,
                                    tSirMacAddr peerMac,
                                    tANI_S32 tdlsOffCh,
                                    tANI_S32 tdlsOffChBwOffset,
                                    tANI_U8 tdlsSwMode)
{
    tpAniSirGlobal pMac = PMAC_STRUCT( hHal );
    tSmeCmd *tdlsChanSwitchCmd;
    eHalStatus status = eHAL_STATUS_FAILURE ;

    //If connected and in Infra. Only then allow this
    if( CSR_IS_SESSION_VALID( pMac, sessionId ) &&
        csrIsConnStateConnectedInfra( pMac, sessionId ) &&
        (NULL != peerMac) )
    {
        tdlsChanSwitchCmd = csrGetCommandBuffer(pMac) ;

        if(tdlsChanSwitchCmd)
        {
            tTdlsChanSwitchCmdInfo *tdlsChanSwitchCmdInfo =
            &tdlsChanSwitchCmd->u.tdlsCmd.u.tdlsChanSwitchCmdInfo;

            vos_mem_zero(&tdlsChanSwitchCmd->u.tdlsCmd, sizeof(tTdlsCmd));

            tdlsChanSwitchCmd->sessionId = sessionId;

            vos_mem_copy(tdlsChanSwitchCmdInfo->peerMac,
                         peerMac, sizeof(tSirMacAddr));
            tdlsChanSwitchCmdInfo->tdlsOffCh = tdlsOffCh;
            tdlsChanSwitchCmdInfo->tdlsOffChBwOffset = tdlsOffChBwOffset;
            tdlsChanSwitchCmdInfo->tdlsSwMode = tdlsSwMode;

            tdlsChanSwitchCmd->command = eSmeCommandTdlsChannelSwitch;
            tdlsChanSwitchCmd->u.tdlsCmd.size = sizeof(tTdlsChanSwitchCmdInfo) ;
            smePushCommand(pMac, tdlsChanSwitchCmd, FALSE) ;
            status = eHAL_STATUS_SUCCESS ;
        }
    }

    return status ;
}


>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#ifdef FEATURE_WLAN_TDLS_INTERNAL
/*
 * TDLS request API, called from HDD to enable TDLS discovery request
 * in SME/CSR and send message to PE to trigger TDLS discovery procedure.
 */
eHalStatus csrTdlsDiscoveryReq(tHalHandle hHal, tANI_U8 sessionId, tCsrTdlsDisRequest *tdlsDisReq)
{
    tpAniSirGlobal pMac = PMAC_STRUCT( hHal );
    tSmeCmd *tdlsDisReqCmd ;
    eHalStatus status = eHAL_STATUS_FAILURE ;
 
    if(tdlsDisReq)
    {
        tdlsDisReqCmd = csrGetCommandBuffer(pMac) ;

        if(tdlsDisReqCmd)
        {
            tTdlsDisReqCmdinfo *disReqCmdInfo = 
                            &tdlsDisReqCmd->u.tdlsCmd.u.tdlsDisReqCmdInfo ;

<<<<<<< HEAD
            tdlsDisReqCmd->sessionId = sessionId;

            disReqCmdInfo->tdlsDisType = tdlsDisReq->disType ;   
            palCopyMemory(pMac->hHdd, disReqCmdInfo->peerMac, 
=======
            vos_mem_zero(&tdlsDisReqCmd->u.tdlsCmd, sizeof(tTdlsCmd));

            tdlsDisReqCmd->sessionId = sessionId;

            disReqCmdInfo->tdlsDisType = tdlsDisReq->disType ;   
            vos_mem_copy(disReqCmdInfo->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                   tdlsDisReq->peerMac, sizeof(tSirMacAddr)) ; 
            tdlsDisReqCmd->command = eSmeCommandTdlsDiscovery ;
            tdlsDisReqCmd->u.tdlsCmd.size = sizeof(tTdlsDisReqCmdinfo) ;
            smePushCommand(pMac, tdlsDisReqCmd, FALSE) ;
            status = eHAL_STATUS_SUCCESS ;
        }
    }

    return status ;
}

/*
 * TDLS request API, called from HDD to enable TDLS link setup request
 * in SME/CSR and send message to PE to trigger TDLS link setup procedure.
 */
eHalStatus csrTdlsSetupReq(tHalHandle hHal, tANI_U8 sessionId, tCsrTdlsSetupRequest *tdlsSetupReq)
{
    tpAniSirGlobal pMac = PMAC_STRUCT( hHal );
    tSmeCmd *tdlsSetupReqCmd ;
    eHalStatus status = eHAL_STATUS_FAILURE ;
 
    if(tdlsSetupReq)
    {
        tdlsSetupReqCmd = csrGetCommandBuffer(pMac) ;

        if(tdlsSetupReqCmd)
        {
           tTdlsLinkSetupReqCmdinfo *setupCmdInfo = 
                        &tdlsSetupReqCmd->u.tdlsCmd.u.tdlsLinkSetupReqCmdInfo ;

<<<<<<< HEAD
            tdlsSetupReqCmd->sessionId = sessionId;

            palCopyMemory(pMac->hHdd, setupCmdInfo->peerMac, 
=======
            vos_mem_zero(&tdlsSetupReqCmd->u.tdlsCmd, sizeof(tTdlsCmd));

            tdlsSetupReqCmd->sessionId = sessionId;

            vos_mem_copy(setupCmdInfo->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                tdlsSetupReq->peerMac, sizeof(tSirMacAddr)) ; 
            tdlsSetupReqCmd->command = eSmeCommandTdlsLinkSetup ;
            tdlsSetupReqCmd->u.tdlsCmd.size = sizeof(tTdlsLinkSetupReqCmdinfo) ;
            smePushCommand(pMac, tdlsSetupReqCmd, FALSE) ;
            status = eHAL_STATUS_SUCCESS ;
        }
    }

    return status ;
}
/*
 * TDLS request API, called from HDD to enable TDLS link teardown request
 * in SME/CSR and send message to PE to trigger TDLS link teardown procedure.
 */
eHalStatus csrTdlsTeardownReq(tHalHandle hHal, tANI_U8 sessionId, 
                                 tCsrTdlsTeardownRequest *tdlsTeardownReq)
{
    tpAniSirGlobal pMac = PMAC_STRUCT( hHal );
    tSmeCmd *tdlsTeardownReqCmd ;
    eHalStatus status = eHAL_STATUS_FAILURE ;
 
    if(tdlsTeardownReq)
    {
        tdlsTeardownReqCmd = csrGetCommandBuffer(pMac) ;
        
        if(tdlsTeardownReqCmd)
        {
            tTdlsLinkTeardownCmdinfo *teardownCmdInfo = 
                   &tdlsTeardownReqCmd->u.tdlsCmd.u.tdlsLinkTeardownCmdInfo ;

<<<<<<< HEAD
            tdlsTeardownReqCmd->sessionId = sessionId;

            palCopyMemory(pMac->hHdd, teardownCmdInfo->peerMac, 
=======
            vos_mem_zero(&tdlsTeardownReqCmd->u.tdlsCmd, sizeof(tTdlsCmd));

            tdlsTeardownReqCmd->sessionId = sessionId;

            vos_mem_copy(teardownCmdInfo->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                         tdlsTeardownReq->peerMac, sizeof(tSirMacAddr)) ; 
            tdlsTeardownReqCmd->command = eSmeCommandTdlsLinkTear ;
            tdlsTeardownReqCmd->u.tdlsCmd.size = 
                                        sizeof(tTdlsLinkTeardownCmdinfo) ;
            smePushCommand(pMac, tdlsTeardownReqCmd, FALSE) ;
            status = eHAL_STATUS_SUCCESS ;
        }
    }

    return status ;
}

#endif

/*
 * TDLS messages sent to PE .
 */
eHalStatus tdlsSendMessage(tpAniSirGlobal pMac, tANI_U16 msg_type, 
                              void *msg_data, tANI_U32 msg_size)
{

    tSirMbMsg *pMsg = (tSirMbMsg *)msg_data ;
    pMsg->type = msg_type ;
    pMsg->msgLen = (tANI_U16) (msg_size) ;

    VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO,
                              ("sending msg = %d"), pMsg->type) ;
      /* Send message. */
    if (palSendMBMessage(pMac->hHdd, pMsg) != eHAL_STATUS_SUCCESS)
    {
        smsLog(pMac, LOGE, FL("Cannot send message"));
        return eHAL_STATUS_FAILURE;
    }

    return eHAL_STATUS_SUCCESS;
}

eHalStatus csrTdlsProcessSendMgmt( tpAniSirGlobal pMac, tSmeCmd *cmd )
{
    tTdlsSendMgmtCmdInfo *tdlsSendMgmtCmdInfo = &cmd->u.tdlsCmd.u.tdlsSendMgmtCmdInfo ;
    tSirTdlsSendMgmtReq *tdlsSendMgmtReq = NULL ;
    tCsrRoamSession *pSession = CSR_GET_SESSION( pMac, cmd->sessionId );
    eHalStatus status = eHAL_STATUS_FAILURE;

<<<<<<< HEAD
=======
    if (NULL == pSession)
    {
        smsLog( pMac, LOGE, FL("pSession is NULL"));
        return eHAL_STATUS_FAILURE;
    }
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
    if (NULL == pSession->pConnectBssDesc)
    {
        smsLog( pMac, LOGE, FL("BSS Description is not present") );
        return eHAL_STATUS_FAILURE;
    }

<<<<<<< HEAD
    status = palAllocateMemory( pMac->hHdd, (void **)&tdlsSendMgmtReq, 
            (sizeof(tSirTdlsSendMgmtReq) + tdlsSendMgmtCmdInfo->len ) );
=======
    tdlsSendMgmtReq = vos_mem_malloc(
                      sizeof(tSirTdlsSendMgmtReq) + tdlsSendMgmtCmdInfo->len);
    if ( NULL == tdlsSendMgmtReq )
       status = eHAL_STATUS_FAILURE;
    else
       status = eHAL_STATUS_SUCCESS;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

    if (!HAL_STATUS_SUCCESS( status ) )
    {
        smsLog( pMac, LOGE, FL("alloc failed") );
        VOS_ASSERT(0) ;
        return status ;
    }
    tdlsSendMgmtReq->sessionId = cmd->sessionId;
    //Using dialog as transactionId. This can be used to match response with request
    tdlsSendMgmtReq->transactionId = tdlsSendMgmtCmdInfo->dialog;  
    tdlsSendMgmtReq->reqType =  tdlsSendMgmtCmdInfo->frameType ;
    tdlsSendMgmtReq->dialog =  tdlsSendMgmtCmdInfo->dialog ;
    tdlsSendMgmtReq->statusCode =  tdlsSendMgmtCmdInfo->statusCode ;
    tdlsSendMgmtReq->responder =  tdlsSendMgmtCmdInfo->responder;
<<<<<<< HEAD

    palCopyMemory(pMac->hHdd, tdlsSendMgmtReq->bssid,
                  pSession->pConnectBssDesc->bssId, sizeof (tSirMacAddr));

    palCopyMemory(pMac->hHdd, tdlsSendMgmtReq->peerMac, 
=======
    tdlsSendMgmtReq->peerCapability = tdlsSendMgmtCmdInfo->peerCapability;

    vos_mem_copy(tdlsSendMgmtReq->bssid,
                  pSession->pConnectBssDesc->bssId, sizeof (tSirMacAddr));

    vos_mem_copy(tdlsSendMgmtReq->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
            tdlsSendMgmtCmdInfo->peerMac, sizeof(tSirMacAddr)) ;

    if(tdlsSendMgmtCmdInfo->len && tdlsSendMgmtCmdInfo->buf)
    {
<<<<<<< HEAD
        palCopyMemory(pMac->hHdd, tdlsSendMgmtReq->addIe, tdlsSendMgmtCmdInfo->buf,
=======
        vos_mem_copy(tdlsSendMgmtReq->addIe, tdlsSendMgmtCmdInfo->buf,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                tdlsSendMgmtCmdInfo->len);

    }
    // Send the request to PE.
    smsLog( pMac, LOG1, "sending TDLS Mgmt Frame req to PE " );
    status = tdlsSendMessage(pMac, eWNI_SME_TDLS_SEND_MGMT_REQ, 
            (void *)tdlsSendMgmtReq , sizeof(tSirTdlsSendMgmtReq)+tdlsSendMgmtCmdInfo->len) ;
    if(!HAL_STATUS_SUCCESS( status ) )
    {
        smsLog( pMac, LOGE, FL("Failed to send request to MAC"));
    }
    if(tdlsSendMgmtCmdInfo->len && tdlsSendMgmtCmdInfo->buf)
    {
        //Done with the buf. Free it.
<<<<<<< HEAD
        palFreeMemory( pMac->hHdd, tdlsSendMgmtCmdInfo->buf );
=======
        vos_mem_free( tdlsSendMgmtCmdInfo->buf );
        tdlsSendMgmtCmdInfo->buf = NULL;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
        tdlsSendMgmtCmdInfo->len = 0;
    }

    return status;
}

eHalStatus csrTdlsProcessAddSta( tpAniSirGlobal pMac, tSmeCmd *cmd )
{
    tTdlsAddStaCmdInfo *tdlsAddStaCmdInfo = &cmd->u.tdlsCmd.u.tdlsAddStaCmdInfo ;
    tSirTdlsAddStaReq *tdlsAddStaReq = NULL ;
    tCsrRoamSession *pSession = CSR_GET_SESSION( pMac, cmd->sessionId );
    eHalStatus status = eHAL_STATUS_FAILURE;

<<<<<<< HEAD
=======
    if (NULL == pSession)
    {
        smsLog( pMac, LOGE, FL("pSession is NULL"));
        return eHAL_STATUS_FAILURE;
    }

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
    if (NULL == pSession->pConnectBssDesc)
    {
        smsLog( pMac, LOGE, FL("BSS description is not present") );
        return eHAL_STATUS_FAILURE;
    }

<<<<<<< HEAD
    status = palAllocateMemory( pMac->hHdd, (void **)&tdlsAddStaReq, 
            (sizeof(tSirTdlsAddStaReq) ) );
=======
    tdlsAddStaReq = vos_mem_malloc(sizeof(tSirTdlsAddStaReq));
    if ( NULL == tdlsAddStaReq )
        status = eHAL_STATUS_FAILURE;
    else
        status = eHAL_STATUS_SUCCESS;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

    if (!HAL_STATUS_SUCCESS( status ) )
    {
        smsLog( pMac, LOGE, FL("alloc failed") );
        VOS_ASSERT(0) ;
        return status ;
    }
<<<<<<< HEAD
=======
    vos_mem_set(tdlsAddStaReq, sizeof(tSirTdlsAddStaReq), 0);

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
    tdlsAddStaReq->sessionId = cmd->sessionId;
    tdlsAddStaReq->tdlsAddOper = tdlsAddStaCmdInfo->tdlsAddOper;
    //Using dialog as transactionId. This can be used to match response with request
    tdlsAddStaReq->transactionId = 0;

<<<<<<< HEAD
    palCopyMemory(pMac->hHdd, tdlsAddStaReq->bssid,
                  pSession->pConnectBssDesc->bssId, sizeof (tSirMacAddr));

    palCopyMemory(pMac->hHdd, tdlsAddStaReq->peerMac, 
=======
    vos_mem_copy( tdlsAddStaReq->bssid,
                  pSession->pConnectBssDesc->bssId, sizeof (tSirMacAddr));

    vos_mem_copy( tdlsAddStaReq->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
            tdlsAddStaCmdInfo->peerMac, sizeof(tSirMacAddr)) ;

    tdlsAddStaReq->capability = tdlsAddStaCmdInfo->capability;
    tdlsAddStaReq->uapsd_queues = tdlsAddStaCmdInfo->uapsdQueues;
    tdlsAddStaReq->max_sp = tdlsAddStaCmdInfo->maxSp;

<<<<<<< HEAD
    palCopyMemory(pMac->hHdd, tdlsAddStaReq->extn_capability,
                              tdlsAddStaCmdInfo->extnCapability,
                              SIR_MAC_MAX_EXTN_CAP);
    tdlsAddStaReq->htcap_present = tdlsAddStaCmdInfo->htcap_present;
    palCopyMemory(pMac->hHdd, &tdlsAddStaReq->htCap,
                  &tdlsAddStaCmdInfo->HTCap, sizeof(tdlsAddStaCmdInfo->HTCap));
    tdlsAddStaReq->vhtcap_present = tdlsAddStaCmdInfo->vhtcap_present;
    palCopyMemory(pMac->hHdd, &tdlsAddStaReq->vhtCap,
                  &tdlsAddStaCmdInfo->VHTCap, sizeof(tdlsAddStaCmdInfo->VHTCap));
    tdlsAddStaReq->supported_rates_length = tdlsAddStaCmdInfo->supportedRatesLen;
    palCopyMemory(pMac->hHdd, &tdlsAddStaReq->supported_rates,
=======
    vos_mem_copy( tdlsAddStaReq->extn_capability,
                              tdlsAddStaCmdInfo->extnCapability,
                              SIR_MAC_MAX_EXTN_CAP);
    tdlsAddStaReq->htcap_present = tdlsAddStaCmdInfo->htcap_present;
    vos_mem_copy( &tdlsAddStaReq->htCap,
                  &tdlsAddStaCmdInfo->HTCap, sizeof(tdlsAddStaCmdInfo->HTCap));
    tdlsAddStaReq->vhtcap_present = tdlsAddStaCmdInfo->vhtcap_present;
    vos_mem_copy( &tdlsAddStaReq->vhtCap,
                  &tdlsAddStaCmdInfo->VHTCap, sizeof(tdlsAddStaCmdInfo->VHTCap));
    tdlsAddStaReq->supported_rates_length = tdlsAddStaCmdInfo->supportedRatesLen;
    vos_mem_copy( &tdlsAddStaReq->supported_rates,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                  tdlsAddStaCmdInfo->supportedRates, tdlsAddStaCmdInfo->supportedRatesLen);

    // Send the request to PE.
    smsLog( pMac, LOGE, "sending TDLS Add Sta req to PE " );
    status = tdlsSendMessage(pMac, eWNI_SME_TDLS_ADD_STA_REQ, 
            (void *)tdlsAddStaReq , sizeof(tSirTdlsAddStaReq)) ;
    if(!HAL_STATUS_SUCCESS( status ) )
    {
        smsLog( pMac, LOGE, FL("Failed to send request to MAC"));
    }
    return status;
}

eHalStatus csrTdlsProcessDelSta( tpAniSirGlobal pMac, tSmeCmd *cmd )
{
    tTdlsDelStaCmdInfo *tdlsDelStaCmdInfo = &cmd->u.tdlsCmd.u.tdlsDelStaCmdInfo ;
    tSirTdlsDelStaReq *tdlsDelStaReq = NULL ;
    tCsrRoamSession *pSession = CSR_GET_SESSION( pMac, cmd->sessionId );
    eHalStatus status = eHAL_STATUS_FAILURE;

<<<<<<< HEAD
=======
    if (NULL == pSession)
    {
        smsLog( pMac, LOGE, FL("pSession is NULL"));
        return eHAL_STATUS_FAILURE;
    }

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
    if (NULL == pSession->pConnectBssDesc)
    {
        smsLog( pMac, LOGE, FL("BSS description is not present") );
        return eHAL_STATUS_FAILURE;
    }

<<<<<<< HEAD
    status = palAllocateMemory( pMac->hHdd, (void **)&tdlsDelStaReq, 
            (sizeof(tSirTdlsDelStaReq) ) );
=======
    tdlsDelStaReq = vos_mem_malloc(sizeof(tSirTdlsDelStaReq));
    if ( NULL == tdlsDelStaReq )
        status = eHAL_STATUS_FAILURE;
    else
        status = eHAL_STATUS_SUCCESS;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

    if (!HAL_STATUS_SUCCESS( status ) )
    {
        smsLog( pMac, LOGE, FL("alloc failed") );
        VOS_ASSERT(0) ;
        return status ;
    }
    tdlsDelStaReq->sessionId = cmd->sessionId;
    //Using dialog as transactionId. This can be used to match response with request
    tdlsDelStaReq->transactionId = 0;

<<<<<<< HEAD
    palCopyMemory(pMac->hHdd, tdlsDelStaReq->bssid,
                  pSession->pConnectBssDesc->bssId, sizeof (tSirMacAddr));

    palCopyMemory(pMac->hHdd, tdlsDelStaReq->peerMac, 
=======
    vos_mem_copy( tdlsDelStaReq->bssid,
                  pSession->pConnectBssDesc->bssId, sizeof (tSirMacAddr));

    vos_mem_copy( tdlsDelStaReq->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
            tdlsDelStaCmdInfo->peerMac, sizeof(tSirMacAddr)) ;

    // Send the request to PE.
#ifdef WLAN_FEATURE_TDLS_DEBUG
    smsLog( pMac, LOGE,
#else
    smsLog( pMac, LOG1,
#endif
<<<<<<< HEAD
        "sending TDLS Del Sta %02x:%02x:%02x:%02x:%02x:%02x req to PE",
        tdlsDelStaCmdInfo->peerMac[0], tdlsDelStaCmdInfo->peerMac[1], tdlsDelStaCmdInfo->peerMac[2],
        tdlsDelStaCmdInfo->peerMac[3], tdlsDelStaCmdInfo->peerMac[4], tdlsDelStaCmdInfo->peerMac[5]);
=======
        "sending TDLS Del Sta "MAC_ADDRESS_STR" req to PE",
         MAC_ADDR_ARRAY(tdlsDelStaCmdInfo->peerMac));
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
    status = tdlsSendMessage(pMac, eWNI_SME_TDLS_DEL_STA_REQ, 
            (void *)tdlsDelStaReq , sizeof(tSirTdlsDelStaReq)) ;
    if(!HAL_STATUS_SUCCESS( status ) )
    {
        smsLog( pMac, LOGE, FL("Failed to send request to MAC"));
    }
    return status;
}
/*
 * commands received from CSR
 */
eHalStatus csrTdlsProcessCmd(tpAniSirGlobal pMac, tSmeCmd *cmd)
{
    eSmeCommandType  cmdType = cmd->command ;
#ifdef FEATURE_WLAN_TDLS_INTERNAL
    tTdlsCmd tdlsCmd = cmd->u.tdlsCmd ;
#endif
    tANI_BOOLEAN status = eANI_BOOLEAN_TRUE;
    switch(cmdType)
    {
        case eSmeCommandTdlsSendMgmt:
        {
            status = csrTdlsProcessSendMgmt( pMac, cmd );
            if(HAL_STATUS_SUCCESS( status ) )
            {
               status = eANI_BOOLEAN_FALSE ;
            }
        }
        break ;
        case eSmeCommandTdlsAddPeer:
        {
            status = csrTdlsProcessAddSta( pMac, cmd );
            if(HAL_STATUS_SUCCESS( status ) )
            {
               status = eANI_BOOLEAN_FALSE ;
            }
        }
        break;
        case eSmeCommandTdlsDelPeer: 
        {
            status = csrTdlsProcessDelSta( pMac, cmd );
            if(HAL_STATUS_SUCCESS( status ) )
            {
               status = eANI_BOOLEAN_FALSE ;
            }
        }
        break;
        case eSmeCommandTdlsLinkEstablish:
        {
            status = csrTdlsProcessLinkEstablish( pMac, cmd );
            if(HAL_STATUS_SUCCESS( status ) )
            {
               status = eANI_BOOLEAN_FALSE ;
            }
<<<<<<< HEAD
	}
	break;
=======
        }
        break;
// tdlsoffchan
        case eSmeCommandTdlsChannelSwitch:
        {
             status = csrTdlsProcessChanSwitchReq( pMac, cmd );
             if(HAL_STATUS_SUCCESS( status ) )
             {
               status = eANI_BOOLEAN_FALSE ;
             }
        }
        break;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#ifdef FEATURE_WLAN_TDLS_INTERNAL
        case eSmeCommandTdlsDiscovery:
        {
            tTdlsDisReqCmdinfo *disReqCmdInfo = &tdlsCmd.u.tdlsDisReqCmdInfo ;
            tSirTdlsDisReq *disReq = NULL ;
            tCsrRoamSession *pSession = CSR_GET_SESSION( pMac, cmd->sessionId );
            
<<<<<<< HEAD
            status = palAllocateMemory( pMac->hHdd, (void **)&disReq, 
                                                      sizeof(tSirTdlsDisReq));
=======
            disReq = vos_mem_malloc(sizeof(tSirTdlsDisReq));
            if ( NULL == disReq )
                status = eHAL_STATUS_FAILURE;
            else
                status = eHAL_STATUS_SUCCESS;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

            if(!HAL_STATUS_SUCCESS( status ) )
            {
                smsLog( pMac, LOGE, "dis Req alloc failed " );
                VOS_ASSERT(0) ;
                break ;
            }
            disReq->sessionId = cmd->sessionId;
            disReq->transactionId = 0;  /* TODO ? */
            disReq->reqType =  disReqCmdInfo->tdlsDisType ;
<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, disReq->bssid, pSession->pConnectBssDesc->bssId, 
                                 sizeof (tSirMacAddr));
            palCopyMemory(pMac->hHdd, disReq->peerMac, 
=======
            vos_mem_copy( disReq->bssid, pSession->pConnectBssDesc->bssId,
                                 sizeof (tSirMacAddr));
            vos_mem_copy( disReq->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                 disReqCmdInfo->peerMac, sizeof(tSirMacAddr)) ;
            smsLog( pMac, LOGE, "sending TDLS discovery to PE " );
            status = tdlsSendMessage(pMac, eWNI_SME_TDLS_DISCOVERY_START_REQ, 
                          (void *)disReq , sizeof(tSirTdlsDisReq)) ;
            if(HAL_STATUS_SUCCESS( status ) )
            {
               status = eANI_BOOLEAN_FALSE ;
            }
                /* TODO: Add error handling */

            break ;
        }
        case eSmeCommandTdlsLinkSetup:
        {
            tTdlsLinkSetupReqCmdinfo *linkSetupReqCmdInfo = 
                                         &tdlsCmd.u.tdlsLinkSetupReqCmdInfo ;   
            tSirTdlsSetupReq *setupReq = NULL ;
            tCsrRoamSession *pSession = CSR_GET_SESSION( pMac, cmd->sessionId );

<<<<<<< HEAD
            status = palAllocateMemory( pMac->hHdd, (void **)&setupReq, 
                                                sizeof(tSirTdlsSetupReq));
=======
            setupReq = vos_mem_malloc(sizeof(tSirTdlsSetupReq));
            if ( NULL == setupReq )
                status = eHAL_STATUS_FAILURE;
            else
                status = eHAL_STATUS_SUCCESS;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

            if(!HAL_STATUS_SUCCESS( status ) )
            {
                smsLog( pMac, LOGE, "dis Req alloc failed " );
                VOS_ASSERT(0) ;
                break ;
            }
            setupReq->sessionId = cmd->sessionId;
            setupReq->transactionId = 0;  /* TODO ? */
            
<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, setupReq->bssid, pSession->pConnectBssDesc->bssId, 
                                 sizeof (tSirMacAddr));
            palCopyMemory(pMac->hHdd, setupReq->peerMac, 
=======
            vos_mem_copy( setupReq->bssid, pSession->pConnectBssDesc->bssId,
                                 sizeof (tSirMacAddr));
            vos_mem_copy( setupReq->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                           linkSetupReqCmdInfo->peerMac, sizeof(tSirMacAddr)) ;
    
            VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
                             ("sending TDLS link setup to PE "));
            status = tdlsSendMessage(pMac, eWNI_SME_TDLS_LINK_START_REQ,    
                          (void *)setupReq , sizeof(tSirTdlsSetupReq) ) ;

            if(HAL_STATUS_SUCCESS( status ) )
            {
               status = eANI_BOOLEAN_FALSE ;
            }
                /* TODO: Add error handling */
            break ;
        }
        case eSmeCommandTdlsLinkTear:
        {
            tTdlsLinkTeardownCmdinfo *linkTeardownCmdInfo = 
                                        &tdlsCmd.u.tdlsLinkTeardownCmdInfo ;
            tSirTdlsTeardownReq *teardownReq = NULL ;
            tCsrRoamSession *pSession = CSR_GET_SESSION( pMac, cmd->sessionId );

<<<<<<< HEAD
            status = palAllocateMemory( pMac->hHdd, (void **)&teardownReq, 
                                                sizeof(tSirTdlsTeardownReq));
=======
            teardownReq = vos_mem_malloc(sizeof(tSirTdlsTeardownReq));
            if ( NULL == teardownReq )
                status = eHAL_STATUS_FAILURE;
            else
                status = eHAL_STATUS_SUCCESS;

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
            if(!HAL_STATUS_SUCCESS( status ) )
            {
                smsLog( pMac, LOGE, "teardown Req alloc failed " );
                VOS_ASSERT(0) ;
                break ;
            }
            teardownReq->sessionId = cmd->sessionId;
            teardownReq->transactionId = 0;  /* TODO ? */

<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, teardownReq->bssid, pSession->pConnectBssDesc->bssId, 
                                 sizeof (tSirMacAddr));
            palCopyMemory(pMac->hHdd, &teardownReq->peerMac, 
=======
            vos_mem_copy( teardownReq->bssid, pSession->pConnectBssDesc->bssId,
                                 sizeof (tSirMacAddr));
            vos_mem_copy( &teardownReq->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                           linkTeardownCmdInfo->peerMac, sizeof(tSirMacAddr)) ;
            VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
                                                ("teardown request..")) ;
            status = tdlsSendMessage(pMac, eWNI_SME_TDLS_TEARDOWN_REQ,
                          (void *)teardownReq , sizeof(tSirTdlsTeardownReq)) ;

            if(HAL_STATUS_SUCCESS( status ) )
            {
               status = eANI_BOOLEAN_FALSE ;
            }
                /* TODO: Add error handling */
            break ;
        }
#endif
       default:
       {
            /* TODO: Add defualt handling */  
           break ;
       } 
             
    }
    return status ; 
}

eHalStatus csrTdlsProcessLinkEstablish( tpAniSirGlobal pMac, tSmeCmd *cmd )
{
    tTdlsLinkEstablishCmdInfo *tdlsLinkEstablishCmdInfo = &cmd->u.tdlsCmd.u.tdlsLinkEstablishCmdInfo ;
    tSirTdlsLinkEstablishReq *tdlsLinkEstablishReq = NULL ;
    eHalStatus status = eHAL_STATUS_FAILURE;
    tCsrRoamSession *pSession = CSR_GET_SESSION( pMac, cmd->sessionId );

<<<<<<< HEAD
    status = palAllocateMemory( pMac->hHdd, (void **)&tdlsLinkEstablishReq,
            (sizeof(tSirTdlsLinkEstablishReq) ) );

    if (!HAL_STATUS_SUCCESS( status ) )
=======
    if (NULL == pSession)
    {
        smsLog( pMac, LOGE, FL("pSession is NULL"));
        return eHAL_STATUS_FAILURE;
    }

    tdlsLinkEstablishReq = vos_mem_malloc(sizeof(tSirTdlsLinkEstablishReq));

    if (tdlsLinkEstablishReq == NULL)
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
    {
        smsLog( pMac, LOGE, FL("alloc failed \n") );
        VOS_ASSERT(0) ;
        return status ;
    }
    tdlsLinkEstablishReq->sessionId = cmd->sessionId;
    //Using dialog as transactionId. This can be used to match response with request
    tdlsLinkEstablishReq->transactionId = 0;
<<<<<<< HEAD
    palCopyMemory(pMac->hHdd, tdlsLinkEstablishReq->peerMac,
                  tdlsLinkEstablishCmdInfo->peerMac, sizeof(tSirMacAddr));
    palCopyMemory(pMac->hHdd, tdlsLinkEstablishReq->bssid, pSession->pConnectBssDesc->bssId,
                  sizeof (tSirMacAddr));
=======
    vos_mem_copy(tdlsLinkEstablishReq->peerMac,
                  tdlsLinkEstablishCmdInfo->peerMac, sizeof(tSirMacAddr));
    vos_mem_copy(tdlsLinkEstablishReq->bssid, pSession->pConnectBssDesc->bssId,
                  sizeof (tSirMacAddr));
    vos_mem_copy(tdlsLinkEstablishReq->supportedChannels,
                  tdlsLinkEstablishCmdInfo->supportedChannels,
                  tdlsLinkEstablishCmdInfo->supportedChannelsLen);
    tdlsLinkEstablishReq->supportedChannelsLen =
                      tdlsLinkEstablishCmdInfo->supportedChannelsLen;
    vos_mem_copy(tdlsLinkEstablishReq->supportedOperClasses,
                  tdlsLinkEstablishCmdInfo->supportedOperClasses,
                  tdlsLinkEstablishCmdInfo->supportedOperClassesLen);
    tdlsLinkEstablishReq->supportedOperClassesLen =
                      tdlsLinkEstablishCmdInfo->supportedOperClassesLen;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
    tdlsLinkEstablishReq->isBufSta = tdlsLinkEstablishCmdInfo->isBufSta;
    tdlsLinkEstablishReq->isResponder= tdlsLinkEstablishCmdInfo->isResponder;
    tdlsLinkEstablishReq->uapsdQueues= tdlsLinkEstablishCmdInfo->uapsdQueues;
    tdlsLinkEstablishReq->maxSp= tdlsLinkEstablishCmdInfo->maxSp;


    // Send the request to PE.
    smsLog( pMac, LOGE, "sending TDLS Link Establish Request to PE \n" );
    status = tdlsSendMessage(pMac, eWNI_SME_TDLS_LINK_ESTABLISH_REQ,
                             (void *)tdlsLinkEstablishReq,
                             sizeof(tSirTdlsLinkEstablishReq));
    if (!HAL_STATUS_SUCCESS( status ) )
    {
        smsLog( pMac, LOGE, FL("Failed to send request to MAC\n"));
    }
    return status;
}

<<<<<<< HEAD
=======
// tdlsoffchan
eHalStatus csrTdlsProcessChanSwitchReq( tpAniSirGlobal pMac, tSmeCmd *cmd )
{
    tTdlsChanSwitchCmdInfo *tdlsChanSwitchCmdInfo = &cmd->u.tdlsCmd.u.tdlsChanSwitchCmdInfo ;
    tSirTdlsChanSwitch *tdlsChanSwitch = NULL ;
    eHalStatus status = eHAL_STATUS_FAILURE;
    tCsrRoamSession *pSession = CSR_GET_SESSION( pMac, cmd->sessionId );

    if (NULL == pSession)
    {
        smsLog( pMac, LOGE, FL("pSession is NULL"));
        return eHAL_STATUS_FAILURE;
    }

    tdlsChanSwitch = vos_mem_malloc(sizeof(tSirTdlsChanSwitch));
    if (tdlsChanSwitch == NULL)
    {
        smsLog( pMac, LOGE, FL("alloc failed \n") );
        VOS_ASSERT(0) ;
        return status ;
    }
    tdlsChanSwitch->sessionId = cmd->sessionId;
    //Using dialog as transactionId. This can be used to match response with request
    tdlsChanSwitch->transactionId = 0;
    vos_mem_copy( tdlsChanSwitch->peerMac,
                  tdlsChanSwitchCmdInfo->peerMac, sizeof(tSirMacAddr));
    vos_mem_copy(tdlsChanSwitch->bssid, pSession->pConnectBssDesc->bssId,
                 sizeof (tSirMacAddr));

    tdlsChanSwitch->tdlsOffCh = tdlsChanSwitchCmdInfo->tdlsOffCh;
    tdlsChanSwitch->tdlsOffChBwOffset = tdlsChanSwitchCmdInfo->tdlsOffChBwOffset;
    tdlsChanSwitch->tdlsSwMode = tdlsChanSwitchCmdInfo->tdlsSwMode;

    // Send the request to PE.
    smsLog( pMac, LOGE, "sending TDLS Channel Switch to PE \n" );
    status = tdlsSendMessage(pMac, eWNI_SME_TDLS_CHANNEL_SWITCH_REQ,
                             (void *)tdlsChanSwitch,
                             sizeof(tSirTdlsChanSwitch));
    if (!HAL_STATUS_SUCCESS( status ) )
    {
        smsLog( pMac, LOGE, FL("Failed to send request to MAC\n"));
    }
    return status;
}

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#ifdef FEATURE_WLAN_TDLS_INTERNAL
/*
 * Find specific TDLS peer (based on peer MAC address).
 */
tCsrTdlsPeerLinkinfo *findTdlsPeer(tpAniSirGlobal pMac, 
                                tDblLinkList *peerList, tSirMacAddr peerMac)
{
    tListElem *pEntry = NULL ;
    tCsrTdlsPeerLinkinfo *peerLinkInfo = NULL ; 
    tSirTdlsPeerInfo *peerInfo = NULL ;

    pEntry = csrLLPeekHead( peerList, LL_ACCESS_LOCK );
    while(pEntry)
    {
        peerLinkInfo = GET_BASE_ADDR( pEntry, tCsrTdlsPeerLinkinfo, 
                                                        tdlsPeerStaLink) ;
        peerInfo = &peerLinkInfo->tdlsDisPeerInfo ;
 
<<<<<<< HEAD
        if(palEqualMemory( pMac->hHdd,(tANI_U8 *)peerInfo->peerMac,
=======
        if ( vos_mem_compare( (tANI_U8 *)peerInfo->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                              (tANI_U8 *) peerMac, sizeof(tSirMacAddr)) )
        {
            break ;
        }

        pEntry = csrLLNext( peerList, pEntry, LL_ACCESS_LOCK) ;
    }
 
    return ((pEntry) ? peerLinkInfo: NULL) ; 
}
/*
 * Delete TDLD peer info, called from TDLS teardown request.
 */
static eHalStatus tdlsDeleteTdlsPeerInfo(tpAniSirGlobal pMac,
                                              tSirMacAddr peerMac)
{
    tCsrTdlsCtxStruct *disInfo = &pMac->tdlsCtx ;
    tCsrTdlsPeerLinkinfo *peerInfo = 
               findTdlsPeer(pMac, &disInfo->tdlsPotentialPeerList, peerMac) ;
    eHalStatus status = eHAL_STATUS_FAILURE ;

    if((csrLLRemoveEntry(&disInfo->tdlsPotentialPeerList, 
                        &peerInfo->tdlsPeerStaLink, LL_ACCESS_NOLOCK)))
    {
<<<<<<< HEAD
       status = palFreeMemory(pMac->hHdd, (void *)peerInfo) ;
=======
       vos_mem_free((void *)peerInfo) ;
       status = eHAL_STATUS_SUCCESS;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

    }

    return status ; 
}

/*
 * UPDATE TDLS peer state to TDLS LINK STATE, this will be called after
 * link is successfully setup with TDLS Peer.
 */
static eHalStatus tdlsUpdateTdlsPeerState(tpAniSirGlobal pMac,
                                          tSirMacAddr peerMac,tANI_U8 state )
{
    tCsrTdlsCtxStruct *disInfo = &pMac->tdlsCtx ;
    tCsrTdlsPeerLinkinfo *peerLinkInfo = 
                findTdlsPeer(pMac, &disInfo->tdlsPotentialPeerList, peerMac) ;
    eHalStatus status = eHAL_STATUS_FAILURE ;

    if(peerLinkInfo)
    {
        tSirTdlsPeerInfo *peerInfo = &peerLinkInfo->tdlsDisPeerInfo ;
        peerInfo->tdlsPeerState = state ;  
        status = eHAL_STATUS_SUCCESS ;
    }
    else
    {
        /* TODO: update this peer found in link setup in peer list */
        VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
                       ("This TDLS client is not in discovery list ") );
        status = eHAL_STATUS_SUCCESS ;
    }

    return status ;
}

/*
 * save TDLS peer info, this will be called after successfull completion
 * of TDLS discovery procedure.
 */
static eHalStatus tdlsSaveTdlsPeerInfo(tpAniSirGlobal pMac, 
                                          tSirTdlsPeerInfo *disPeerInfo)
{
    tCsrTdlsPeerLinkinfo *peerInfo = NULL ; 
    tCsrTdlsCtxStruct *disInfo = &pMac->tdlsCtx ;
    eHalStatus status = eHAL_STATUS_FAILURE ;
    /* 
     * Ok, allocate memory for peer info here
     * we allocate memory for each peer here and free his memory
     * at the time the peer node is getting deleted, possible case is 
     * teardown
     */
<<<<<<< HEAD
    status = palAllocateMemory( pMac->hHdd, (void **)&peerInfo, 
                                sizeof(tCsrTdlsPeerLinkinfo));
=======
    peerInfo = vos_mem_malloc(sizeof(tCsrTdlsPeerLinkinfo));
    if ( NULL = peerInfo )
        status = eHAL_STATUS_FAILURE;
    else
        status = eHAL_STATUS_SUCCESS;
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

    /* 
     * go ahead and copy peerInfo and insert this node info discovery rsp
     * database.
     */ 
    if (HAL_STATUS_SUCCESS(status))
    {
<<<<<<< HEAD
        palZeroMemory(pMac->hHdd, &peerInfo->tdlsDisPeerInfo, 
                                            sizeof(tSirTdlsPeerInfo));
        palCopyMemory(pMac->hHdd, &peerInfo->tdlsDisPeerInfo, disPeerInfo, 
=======
        vos_mem_set( &peerInfo->tdlsDisPeerInfo,
                                            sizeof(tSirTdlsPeerInfo), 0);
        vos_mem_copy( &peerInfo->tdlsDisPeerInfo, disPeerInfo,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                             sizeof(tSirTdlsPeerInfo));

        /*
         * update TDLS client count to indicate there is tdls client
         * in tdls potential peer list.
         */
        disInfo->tdlsPeerCount++ ;

        /*
         * finally insert this tdls peer info into tdls potential peer list
         */
        csrLLInsertTail( &disInfo->tdlsPotentialPeerList, 
                                 &peerInfo->tdlsPeerStaLink, LL_ACCESS_LOCK );
    }

    return status ;

}

#endif
/*
 * TDLS Message processor, will be called after TDLS message recieved from
 * PE
 */
eHalStatus tdlsMsgProcessor(tpAniSirGlobal pMac,  v_U16_t msgType,
                                void *pMsgBuf)
{
    switch(msgType)
    {
        case eWNI_SME_TDLS_SEND_MGMT_RSP:
        {
            /* remove pending eSmeCommandTdlsDiscovery command */
            csrTdlsRemoveSmeCmd(pMac, eSmeCommandTdlsSendMgmt) ;
        }
        break;
        case eWNI_SME_TDLS_ADD_STA_RSP:
        {
            tSirTdlsAddStaRsp *addStaRsp = (tSirTdlsAddStaRsp *) pMsgBuf ;
            eCsrRoamResult roamResult ;
            tCsrRoamInfo roamInfo = {0} ;
<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, &roamInfo.peerMac, addStaRsp->peerMac, 
=======
            vos_mem_copy( &roamInfo.peerMac, addStaRsp->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                         sizeof(tSirMacAddr)) ;
            roamInfo.staId = addStaRsp->staId ;
            roamInfo.ucastSig = addStaRsp->ucastSig ;
            roamInfo.bcastSig = addStaRsp->bcastSig ;
            roamInfo.statusCode = addStaRsp->statusCode ;
            /*
             * register peer with TL, we have to go through HDD as this is
             * the only way to register any STA with TL.
             */
            if (addStaRsp->tdlsAddOper == TDLS_OPER_ADD)
                roamResult = eCSR_ROAM_RESULT_ADD_TDLS_PEER;
            else /* addStaRsp->tdlsAddOper must be TDLS_OPER_UPDATE */
                roamResult = eCSR_ROAM_RESULT_UPDATE_TDLS_PEER;
            csrRoamCallCallback(pMac, addStaRsp->sessionId, &roamInfo, 0, 
                                eCSR_ROAM_TDLS_STATUS_UPDATE,
                                roamResult);

            /* remove pending eSmeCommandTdlsDiscovery command */
            csrTdlsRemoveSmeCmd(pMac, eSmeCommandTdlsAddPeer) ;
        }
        break;
        case eWNI_SME_TDLS_DEL_STA_RSP:
        {
            tSirTdlsDelStaRsp *delStaRsp = (tSirTdlsDelStaRsp *) pMsgBuf ;
            tCsrRoamInfo roamInfo = {0} ;

<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, &roamInfo.peerMac, delStaRsp->peerMac, 
=======
            vos_mem_copy( &roamInfo.peerMac, delStaRsp->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                         sizeof(tSirMacAddr)) ;
            roamInfo.staId = delStaRsp->staId ;
            roamInfo.statusCode = delStaRsp->statusCode ;
            /*
             * register peer with TL, we have to go through HDD as this is
             * the only way to register any STA with TL.
             */
            csrRoamCallCallback(pMac, delStaRsp->sessionId, &roamInfo, 0, 
                         eCSR_ROAM_TDLS_STATUS_UPDATE, 
                               eCSR_ROAM_RESULT_DELETE_TDLS_PEER);

            csrTdlsRemoveSmeCmd(pMac, eSmeCommandTdlsDelPeer) ;
        }
        break;
        case eWNI_SME_TDLS_DEL_STA_IND:
        {
            tpSirTdlsDelStaInd pSirTdlsDelStaInd = (tpSirTdlsDelStaInd) pMsgBuf ;
            tCsrRoamInfo roamInfo = {0} ;
<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, &roamInfo.peerMac, pSirTdlsDelStaInd->peerMac,
=======
            vos_mem_copy( &roamInfo.peerMac, pSirTdlsDelStaInd->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                         sizeof(tSirMacAddr)) ;
            roamInfo.staId = pSirTdlsDelStaInd->staId ;
            roamInfo.reasonCode = pSirTdlsDelStaInd->reasonCode ;

            /* Sending the TEARDOWN indication to HDD. */
            csrRoamCallCallback(pMac, pSirTdlsDelStaInd->sessionId, &roamInfo, 0,
                         eCSR_ROAM_TDLS_STATUS_UPDATE,
                               eCSR_ROAM_RESULT_TEARDOWN_TDLS_PEER_IND);
            break ;
        }
<<<<<<< HEAD
#ifdef FEATURE_WLAN_TDLS_OXYGEN_DISAPPEAR_AP
        case eWNI_SME_TDLS_AP_DISAPPEAR_IND:
        {
            tpSirTdlsDisappearAPInd pSirTdlsDisappearAPInd = (tpSirTdlsDisappearAPInd) pMsgBuf;
            tCsrRoamInfo roamInfo = {0} ;
            roamInfo.staId = pSirTdlsDisappearAPInd->staId ;
            /* Sending the TEARDOWN indication to HDD. */
            csrRoamCallCallback(pMac, pSirTdlsDisappearAPInd->sessionId, &roamInfo, 0,
                         eCSR_ROAM_TDLS_STATUS_UPDATE,
                               eCSR_ROAM_RESULT_TDLS_DISAPPEAR_AP_IND);
            break ;
        }
#endif
=======
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
        case eWNI_SME_TDLS_DEL_ALL_PEER_IND:
        {
            tpSirTdlsDelAllPeerInd pSirTdlsDelAllPeerInd = (tpSirTdlsDelAllPeerInd) pMsgBuf ;
            tCsrRoamInfo roamInfo = {0} ;

            /* Sending the TEARDOWN indication to HDD. */
            csrRoamCallCallback(pMac, pSirTdlsDelAllPeerInd->sessionId, &roamInfo, 0,
                                eCSR_ROAM_TDLS_STATUS_UPDATE,
                                eCSR_ROAM_RESULT_DELETE_ALL_TDLS_PEER_IND);
            break ;
        }
        case eWNI_SME_MGMT_FRM_TX_COMPLETION_IND:
        {
            tpSirMgmtTxCompletionInd pSirTdlsDelAllPeerInd = (tpSirMgmtTxCompletionInd) pMsgBuf ;
            tCsrRoamInfo roamInfo = {0} ;
            roamInfo.reasonCode = pSirTdlsDelAllPeerInd->txCompleteStatus;

            csrRoamCallCallback(pMac, pSirTdlsDelAllPeerInd->sessionId, &roamInfo,
                                0, eCSR_ROAM_RESULT_MGMT_TX_COMPLETE_IND, 0);
            break;
        }
        case eWNI_SME_TDLS_LINK_ESTABLISH_RSP:
<<<<<<< HEAD
	{
            tSirTdlsLinkEstablishReqRsp *linkEstablishReqRsp = (tSirTdlsLinkEstablishReqRsp *) pMsgBuf ;
            tCsrRoamInfo roamInfo = {0} ;
#if 0
            palCopyMemory(pMac->hHdd, &roamInfo.peerMac, delStaRsp->peerMac,
=======
        {
            tSirTdlsLinkEstablishReqRsp *linkEstablishReqRsp = (tSirTdlsLinkEstablishReqRsp *) pMsgBuf ;
            tCsrRoamInfo roamInfo = {0} ;
#if 0
            vos_mem_copy(&roamInfo.peerMac, delStaRsp->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                         sizeof(tSirMacAddr)) ;
            roamInfo.staId = delStaRsp->staId ;
            roamInfo.statusCode = delStaRsp->statusCode ;
#endif
            csrRoamCallCallback(pMac, linkEstablishReqRsp->sessionId, &roamInfo, 0,
                         eCSR_ROAM_TDLS_STATUS_UPDATE,
                               eCSR_ROAM_RESULT_LINK_ESTABLISH_REQ_RSP);
            /* remove pending eSmeCommandTdlsLinkEstablish command */
            csrTdlsRemoveSmeCmd(pMac, eSmeCommandTdlsLinkEstablish);
            break;
        }
<<<<<<< HEAD
=======

        case eWNI_SME_TDLS_CHANNEL_SWITCH_RSP:
        {
#if 0
            tSirTdlsChanSwitchReqRsp *ChanSwitchReqRsp = (tSirTdlsChanSwitchReqRsp *) pMsgBuf ;
            tCsrRoamInfo roamInfo = {0} ;
            vos_mem_copy(&roamInfo.peerMac, delStaRsp->peerMac,
                                         sizeof(tSirMacAddr)) ;
            roamInfo.staId = delStaRsp->staId ;
            roamInfo.statusCode = delStaRsp->statusCode ;
            csrRoamCallCallback(pMac, ChanSwitchReqRsp->sessionId, &roamInfo, 0,
                                eCSR_ROAM_TDLS_STATUS_UPDATE,
                                eCSR_ROAM_RESULT_LINK_ESTABLISH_REQ_RSP);
#endif
            /* remove pending eSmeCommandTdlsChanSwitch command */
            csrTdlsRemoveSmeCmd(pMac, eSmeCommandTdlsChannelSwitch);
            break;
        }
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#ifdef FEATURE_WLAN_TDLS_INTERNAL
        case eWNI_SME_TDLS_DISCOVERY_START_RSP:
        {
            /* remove pending eSmeCommandTdlsDiscovery command */
            csrTdlsRemoveSmeCmd(pMac, eSmeCommandTdlsDiscovery) ;
        }
        /* fall through .. */
        case eWNI_SME_TDLS_DISCOVERY_START_IND:
        {
            tSirTdlsDisRsp *disRsp = (tSirTdlsDisRsp *)pMsgBuf ;

            if(eSIR_SME_SUCCESS == disRsp->statusCode)
            {
                tCsrTdlsCtxStruct *disInfo = &pMac->tdlsCtx ;
                tANI_U16  disStaCount = disRsp->numDisSta ;
                tCsrTdlsPeerLinkinfo *peerLinkInfo = NULL ;
                tANI_U8 i = 0 ;
  
                VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
                ("DIS START RSP/IND recieved sta count = %d"), disStaCount) ;
                for( ; i < disStaCount ; i++)
                {
                    tSirTdlsPeerInfo *peerInfo = &disRsp->tdlsDisPeerInfo[i] ;
                    VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
<<<<<<< HEAD
                                                   ("SME, peer MAC:")) ;
                    VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
                                    (" %02x,%02x,%02x,%02x,%02x,%02x"), 
                                          peerInfo->peerMac[0], 
                                          peerInfo->peerMac[1], 
                                          peerInfo->peerMac[2], 
                                          peerInfo->peerMac[3], 
                                          peerInfo->peerMac[4], 
                                          peerInfo->peerMac[5]) ;
=======
                              ("SME, peer MAC: "MAC_ADDRESS_STR),
                               MAC_ADDR_ARRAY(peerInfo->peerMac));
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68

                    peerLinkInfo = findTdlsPeer(pMac,
                                   &disInfo->tdlsPotentialPeerList,
                                                     peerInfo->peerMac) ; 

                    if(NULL == peerLinkInfo)
                    {
                        /* update discovery data base, if this is new entry */
                        tdlsSaveTdlsPeerInfo(pMac, peerInfo) ;
                    }
                    else
                    {
                        /* update RSSI of existing peer */
                        tSirTdlsPeerInfo *newPeerInfo = 
                                            &peerLinkInfo->tdlsDisPeerInfo ; 
                        newPeerInfo->tdlsPeerRssi = peerInfo->tdlsPeerRssi ;
                    }
                }
                if(0 == i)
                {
                    smsLog( pMac, LOGW, "there is no tdls client \
                                                      discovered .." ) ;
                }
            }
            else
            {
                smsLog( pMac, LOGW, "there is no tdls client discovered .." );
            }
            /* Now free the memory for this message */
            break ;    
        }    
        case eWNI_SME_TDLS_LINK_START_RSP:
        {
            /* remove pending eSmeCommandTdlsLinkSetup command */
            csrTdlsRemoveSmeCmd(pMac, eSmeCommandTdlsLinkSetup) ;
        }
        /* fall through .. */
        case eWNI_SME_TDLS_LINK_START_IND:
        {
            tSirTdlsLinksetupRsp *linkSetupRsp = 
                                        (tSirTdlsLinksetupRsp *) pMsgBuf ;

            if(eSIR_SME_SUCCESS == linkSetupRsp->statusCode)
            {
                VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
<<<<<<< HEAD
                      ("Link setup for Peer %02x,%02x,%02x,%02x,%02x,%02x"),
                                 linkSetupRsp->peerMac[0],       
                                 linkSetupRsp->peerMac[1],       
                                 linkSetupRsp->peerMac[2],       
                                 linkSetupRsp->peerMac[3],       
                                 linkSetupRsp->peerMac[4],       
                                 linkSetupRsp->peerMac[5]) ;
=======
                      ("Link setup for Peer "MAC_ADDRESS_STR),
                                 MAC_ADDR_ARRAY(linkSetupRsp->peerMac));
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
       
                tdlsUpdateTdlsPeerState(pMac, linkSetupRsp->peerMac, 
                                                  TDLS_LINK_SETUP_STATE) ;
            }
            else
            {
                smsLog( pMac, LOGW, "failure receieved in link setup .." );
            }
            break ;       
        }

        case eWNI_SME_TDLS_TEARDOWN_RSP:
        {
            VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
                                          ("TEARDOWN RSP from PE ")) ;
            /* remove pending eSmeCommandTdlsLinkTear command */
            csrTdlsRemoveSmeCmd(pMac, eSmeCommandTdlsLinkTear) ;
        }
        /* fall through .. */
        case eWNI_SME_TDLS_TEARDOWN_IND:
        {
            tSirTdlsTeardownRsp *linkTearRsp = 
                                        (tSirTdlsTeardownRsp *) pMsgBuf ;
            if(eSIR_SME_SUCCESS == linkTearRsp->statusCode)
            {
            
                VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
<<<<<<< HEAD
                 ("Teardown peer MAC = %02x,%02x,%02x,%02x,%02x,%02x"),
                            linkTearRsp->peerMac[0],  
                            linkTearRsp->peerMac[1],  
                            linkTearRsp->peerMac[2],  
                            linkTearRsp->peerMac[3],  
                            linkTearRsp->peerMac[4],  
                            linkTearRsp->peerMac[5]) ;  
=======
                 ("Teardown peer MAC = "MAC_ADDRESS_STR),
                            MAC_ADDR_ARRAY(linkTearRsp->peerMac));
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                tdlsDeleteTdlsPeerInfo(pMac, linkTearRsp->peerMac) ;
            }
            else
            {
                VOS_TRACE(VOS_MODULE_ID_SME, VOS_TRACE_LEVEL_INFO, 
                                   ("failure receieved in Teardown .." ));
            }
            break ;
        }
        case eWNI_SME_ADD_TDLS_PEER_IND:
        {
            tSirTdlsPeerInd *peerInd = (tSirTdlsPeerInd *) pMsgBuf ;
            tCsrRoamInfo roamInfo = {0} ;
<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, &roamInfo.peerMac, peerInd->peerMac, 
=======
            vos_mem_copy( &roamInfo.peerMac, peerInd->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                         sizeof(tSirMacAddr)) ;
            roamInfo.staId = peerInd->staId ;
            roamInfo.ucastSig = peerInd->ucastSig ;
            roamInfo.bcastSig = peerInd->bcastSig ;
            /*
             * register peer with TL, we have to go through HDD as this is
             * the only way to register any STA with TL.
             */
            csrRoamCallCallback(pMac, peerInd->sessionId, &roamInfo, 0, 
                         eCSR_ROAM_TDLS_STATUS_UPDATE, 
                               eCSR_ROAM_RESULT_ADD_TDLS_PEER);
            break ;
 
        }
        case eWNI_SME_DELETE_TDLS_PEER_IND:
        {
            tSirTdlsPeerInd *peerInd = (tSirTdlsPeerInd *) pMsgBuf ;
            tCsrRoamInfo roamInfo = {0} ;
<<<<<<< HEAD
            palCopyMemory(pMac->hHdd, &roamInfo.peerMac, peerInd->peerMac, 
=======
            vos_mem_copy( &roamInfo.peerMac, peerInd->peerMac,
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
                                         sizeof(tSirMacAddr)) ;
            roamInfo.staId = peerInd->staId ;
            /*
             * register peer with TL, we have to go through HDD as this is
             * the only way to register any STA with TL.
             */
            csrRoamCallCallback(pMac, peerInd->sessionId, &roamInfo, 0, 
                         eCSR_ROAM_TDLS_STATUS_UPDATE, 
                               eCSR_ROAM_RESULT_DELETE_TDLS_PEER);
            break ;

        }
#endif
        default:
        {
            break ;
        }
    }
    
    return eHAL_STATUS_SUCCESS ;
}
#endif
