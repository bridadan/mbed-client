/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed-client/m2mvector.h"
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"

/**
 * @brief M2MTLVSerializer
 * TLV Serialiser constructs the binary representation of object instances,
 * resources and resource instances (see OMA-LWM2M specification, chapter 6.1
 * for resource model) as OMA-TLV according described in chapter 6.3.3. 
 * 
 */
class M2MTLVSerializer {

public:

    /**
     * Serialises given objects instances that contain resources or multiple 
     * resources. Object instance IDs are also encoded. This method must be 
     * used when an operation targets an object with (potential) multiple 
     * instances like "GET /1". In that case the generated TLV will contain the
     * following data:
     * <ul>
     * <li> ./0
     * <li> ./0/0
     * <li> ./0/1
     * <li> ...
     * <li> ./1
     * <li> ./1/0
     * <li> ./1/1
     * <li> ...
     * </ul>
     *    
     * @param objects List of object instances.
     * @return Object instances encoded binary as OMA-TLV 
     * @see #serializeObjectInstances(List) 
     */
    static uint8_t* serialize(const M2MObjectInstanceList &object_instance_list, uint32_t &size);

    /**
     * Serialises given resources with no information about the parent object
     * instance. This method must be used when an operation targets an object
     * instance like "GET /1/0" or a single-instance object like "GET /3//".
     * Resources may have single or multiple instances. The generated TLV will 
     * contain the following data as response to "GET /3//":
     * <ul>
     * <li> ./0
     * <li> ./1
     * <li> ./2
     * <li> ./6/0 (1st instance of a multiple resource)
     * <li> ./6/1 (2nd instance of a multiple resource)
     * <li> ...
     * </ul>
     * @param resources Array of resources and resource instances.
     * @return Resources encoded binary as OMA-TLV
     * @see #serializeResources(List)
     */
    static uint8_t* serialize(const M2MResourceList &resource_list, uint32_t &size);

    static uint8_t* serialize(const M2MResource *resource, uint32_t &size);

private :

    static uint8_t* serialize_object_instances(const M2MObjectInstanceList &object_instance_list, uint32_t &size);

    static uint8_t* serialize_resources(const M2MResourceList &resource_list, uint32_t &size, bool &valid);

    static bool serialize(uint16_t id, const M2MObjectInstance *object_instance, uint8_t *&data, uint32_t &size);
    
    static bool serialize(const M2MResource *resource, uint8_t *&data, uint32_t &size);

    static bool serialize_resource(const M2MResource *resource, uint8_t *&data, uint32_t &size);

    static bool serialize_multiple_resource(const M2MResource *resource, uint8_t *&data, uint32_t &size);

    static bool serialize_resource_instance(uint16_t id, const M2MResourceInstance *resource, uint8_t *&data, uint32_t &size);
    
    static bool serialize_TILV (uint8_t type, uint16_t id, uint8_t *value, uint32_t value_length, uint8_t *&data, uint32_t &size);

    static void serialize_id(uint16_t id, uint32_t &size, uint8_t *id_ptr);

    static void serialize_length(uint32_t length, uint32_t &size, uint8_t *length_ptr);

    static bool serialize_TLV_binary_int(const M2MResourceInstance *resource, uint8_t type, uint16_t id, uint8_t *&data, uint32_t &size);
};
