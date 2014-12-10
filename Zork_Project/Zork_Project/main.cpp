//
//  main.cpp
//  Zork
//
//  Created by b12a on 12/3/14.
//  Copyright (c) 2014 b12a. All rights reserved.
//

#include <iostream>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "zork_header.h"
#include <vector> 

int main(int argc, const char * argv[]) {
	// LOAD IN THE FILE / SET ROOT NODE
	rapidxml::file<> xmlFile(argv[1]);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	rapidxml::xml_node<>* root_node;
	root_node = doc.first_node("map");
	vector<room_object> rooms;

	//GAME INITIALIZATION
	rapidxml::xml_node<>* room_node;
	rapidxml::xml_node<>* name_node;
	rapidxml::xml_node<>* description;
	rapidxml::xml_node<>* border;
	rapidxml::xml_node<>* border_name;
	rapidxml::xml_node<>* border_direction;
	rapidxml::xml_node<>* item_node;
	rapidxml::xml_node<>* trigger_node;
	rapidxml::xml_node<>* condition_node;
	rapidxml::xml_node<>* command_node;
	rapidxml::xml_node<>* trigger_type_node;
	rapidxml::xml_node<>* condition_object;
	rapidxml::xml_node<>* condition_status;
	rapidxml::xml_node<>* condition_has;
	rapidxml::xml_node<>* condition_owner;
	rapidxml::xml_node<>* container_node;
	rapidxml::xml_node<>* creature_node;
	rapidxml::xml_node<>* condition_print;
	rapidxml::xml_node<>* turn_on_node;
	rapidxml::xml_node<>* turn_on_action;
	rapidxml::xml_node<>* accept_node;
	rapidxml::xml_node<>* vulnerability_node;
	rapidxml::xml_node<>* attack_node;

	//GET LIST OF ROOMS AND ALL ITEMS INSIDE OF THEM
	room_node = root_node->first_node("room");
	while (room_node != 0){
		room_object  room;   //REMEMBER TO ADD THIS TO THE OVERALL VECTOR OF ROOMS BEFORE LOOP ITERATES AGAIN
		
		//GET THE NAME OF ALL OF THE ROOMS AND THEIR DESCRIPTIONS
		name_node = room_node->first_node("name");
		description = room_node->first_node("description");
		room.init_name_and_description(name_node->value(), description->value());
		//printf("Room Name: %s\nRoom description: %s\n", name_node->value(), description->value());

		//GET THE BORDERS OF ALL OF THE ROOMS
		border = room_node->first_node("border");
		while (border != 0){
			border_name = border->first_node("name");
			border_direction = border->first_node("direction");
			room.set_borders(border_name->value(), border_direction->value());
			//printf("%s is to the %s\n", border_name->value(), border_direction->value());
			border = border->next_sibling("border");
		}

		//GET THE ITEMS IN EVERY ROOM
		item_node = room_node->first_node("item");
		while (item_node != 0){
			room.get_items(item_node->value());
			//printf("Item %s is in the room.\n", item_node->value());
			item_node = item_node->next_sibling("item");
		}

		//GET THE CONTAINERS IN EVERY ROOM
		container_node = room_node->first_node("container");
		while (container_node != 0){
			room.get_containers(container_node->value());
			//printf("Container %s is in the room. \n", container_node->value());
			container_node = container_node->next_sibling("container");
		}

		//GET THE CREATURES IN EVERY ROOM
		creature_node = room_node->first_node("creature");
		while (creature_node != 0){
			room.get_creatures(creature_node->value());
			//printf("Creature %s is in the room.\n", creature_node->value());
			creature_node = creature_node->next_sibling("creature");
		}
		
		//GET ALL OF THE TRIGGERS IN THE ROOM
		trigger_node = room_node->first_node("trigger");
		if (trigger_node != 0){
			trigger_type_node = trigger_node->first_node("type");
			command_node = trigger_node->first_node("command");
			printf("The trigger is of type %s, with command %s\n", trigger_type_node->value(), command_node->value());
			condition_node = trigger_node->first_node("condition");
			if (condition_node != 0){
				condition_print = trigger_node->first_node("print");
				condition_has = condition_node->first_node("has");
				if (condition_has != 0){
					condition_object = condition_node->first_node("object");
					condition_owner = condition_node->first_node("owner");
					printf("If %s %s has %s, then print %s\n", condition_owner->value(), condition_has->value(), condition_object->value(), condition_print->value());
				}
				else{
					condition_object = condition_node->first_node("object");
					condition_status = condition_node->first_node("status");
					printf("If %s is %s then print %s \n", condition_object->value(), condition_status->value(), condition_print->value());
				}
			}
			trigger_node = trigger_node->next_sibling("trigger");
		}
		rooms.push_back(room);  //ADDS THE CURRENT ROOM INTO THE ROOM VECTOR FOR STORAGE
		//MOVE ON TO THE NEXT ROOM
		room_node = room_node->next_sibling("room");
	}


	//GET ALL ITEM INFO
	item_node = root_node->first_node("item");
	while (item_node != 0){
		name_node = item_node->first_node("name");
		description = item_node->first_node("writing");
		printf("The item is a %s with the writing %s\n", name_node->value(), description->value());
		condition_status = item_node->first_node("status");
		if (condition_status != 0){
			printf("The item is %s\n", condition_status->value());
			turn_on_node = item_node->first_node("turnon");
			condition_print = turn_on_node->first_node("print");
			turn_on_action = turn_on_node->first_node("action");
			printf("If item is turned on print %s and perform %s\n", condition_print->value(), turn_on_action->value());
		}
		item_node = item_node->next_sibling("item");
	}

	//GET ALL CONTAINER INFO
	container_node = root_node->first_node("container");
	while (container_node != 0){
		name_node = container_node->first_node("name");
		item_node = container_node->first_node("item");
		if (item_node != 0){
			printf("Container %s contains %s\n", name_node->value(), item_node->value());
		}
		else{
			condition_status = container_node->first_node("status");
			accept_node = container_node->first_node("accept");
			trigger_node = container_node->first_node("trigger");
			condition_node = trigger_node->first_node("condition");
			condition_has = condition_node->first_node("has");
			condition_object = condition_node->first_node("object");
			condition_owner = condition_node->first_node("owner");
			condition_print = trigger_node->first_node("print");
			turn_on_action = trigger_node->first_node("action");
			printf("The %s is %s but if you put in %s then it %s has a %s in %s and you will print %s and execute %s\n", name_node->value(), condition_status->value(), accept_node->value(), condition_has->value(), condition_object->value(), condition_owner->value(), condition_print->value(), turn_on_action->value());

		}

		container_node = container_node->next_sibling("container");
	}

	//GET ALL CREATURE INFO
	creature_node = root_node->first_node("creature");
	while (creature_node != 0){
		//GET ALL OF THE VULNERABILITY OUT OF HERE LIKE WHAT THE FUCK HAPPENS IF YOU GET ATTACKED LIKE OMFG LIKE WHAT WOULD YOU DO LIKE I WOULD LIKE TOTALLY DIE AND STUFF
		name_node = creature_node->first_node("name");
		vulnerability_node = creature_node->first_node("vulnerability");
		attack_node = creature_node->first_node("attack");
		condition_node = attack_node->first_node("condition");
		condition_object = condition_node->first_node("object");
		condition_status = condition_node->first_node("status");
		condition_print = attack_node->first_node("print");
		printf("Because %s is vulnerable to %s when %s is attacked with %s with status %s the statement: %s is printed and the follwing are performed:\n", name_node->value(), vulnerability_node->value(), name_node->value(), condition_object->value(), condition_status->value(), condition_print->value());
		turn_on_action = attack_node->first_node("action");
		while (turn_on_action != 0){
			printf("%s\n", turn_on_action->value());
			turn_on_action = turn_on_action->next_sibling("action");
		}
		//OH GOD, LIKE SUCH TRIGGER, I MEAN SO MUCH WOW.
		trigger_node = creature_node->first_node("trigger");
		trigger_type_node = trigger_node->first_node("type");
		condition_node = trigger_node->first_node("condition");
		condition_object = condition_node->first_node("object");
		condition_status = condition_node->first_node("status");
		condition_print = trigger_node->first_node("print");
		printf("Additionally, there is a %s trigger where if a %s is %s the following is printed: %s \n", trigger_type_node->value(), condition_object->value(), condition_status->value(), condition_print->value());


		creature_node = creature_node->next_sibling("creature");
	}
	return 0;
}