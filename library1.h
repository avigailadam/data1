//
// Created by avi on 28/11/2021.
//

#ifndef HW1_LIBRARY1_H
#define HW1_LIBRARY1_H

/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1
#define _234218_WET1

#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;


void *Init();

StatusType AddGroup(void *DS, int GroupID);

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level);

StatusType RemovePlayer(void *DS, int PlayerID);

StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID);

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease);

StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID);

StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers);

StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players);

void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET1 */


#endif //HW1_LIBRARY1_H
