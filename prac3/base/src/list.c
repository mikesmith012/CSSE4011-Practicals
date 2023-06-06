#include "list.h"

extern sys_slist_t static_node_list;
extern sys_slist_t all_static_node_list;

StaticNode node_a;
StaticNode node_b;
StaticNode node_c;
StaticNode node_d;
StaticNode node_e;
StaticNode node_f;
StaticNode node_g;
StaticNode node_h;
StaticNode node_i;
StaticNode node_j;
StaticNode node_k;
StaticNode node_l;

StaticNodeID node_id_a;
StaticNodeID node_id_b;
StaticNodeID node_id_c;
StaticNodeID node_id_d;
StaticNodeID node_id_e;
StaticNodeID node_id_f;
StaticNodeID node_id_g;
StaticNodeID node_id_h;
StaticNodeID node_id_i;
StaticNodeID node_id_j;
StaticNodeID node_id_k;
StaticNodeID node_id_l;

void init_list(void) {
    sys_slist_init(&static_node_list);
    sys_slist_init(&all_static_node_list);
    
    
    // init default static nodes (A - H)
    
    sprintf(node_a.name, A_NAME);
    sprintf(node_a.addr, A_ADDR);
    node_a.major = A_MAJOR;
    node_a.minor = A_MINOR;
    node_a.x = A_X;
    node_a.y = A_Y;
    sprintf(node_a.left, B_NAME);
    sprintf(node_a.right, H_NAME);
    
    sprintf(node_b.name, B_NAME);
    sprintf(node_b.addr, B_ADDR);
    node_b.major = B_MAJOR;
    node_b.minor = B_MINOR;
    node_b.x = B_X;
    node_b.y = B_Y;
    sprintf(node_b.left, C_NAME);
    sprintf(node_b.right, A_NAME);
    
    sprintf(node_c.name, C_NAME);
    sprintf(node_c.addr, C_ADDR);
    node_c.major = C_MAJOR;
    node_c.minor = C_MINOR;
    node_c.x = C_X;
    node_c.y = C_Y;
    sprintf(node_c.left, D_NAME);
    sprintf(node_c.right, B_NAME);
    
    sprintf(node_d.name, D_NAME);
    sprintf(node_d.addr, D_ADDR);
    node_d.major = D_MAJOR;
    node_d.minor = D_MINOR;
    node_d.x = D_X;
    node_d.y = D_Y;
    sprintf(node_d.left, E_NAME);
    sprintf(node_d.right, C_NAME);
    
    sprintf(node_e.name, E_NAME);
    sprintf(node_e.addr, E_ADDR);
    node_e.major = E_MAJOR;
    node_e.minor = E_MINOR;
    node_e.x = E_X;
    node_e.y = E_Y;
    sprintf(node_e.left, F_NAME);
    sprintf(node_e.right, D_NAME);
    
    sprintf(node_f.name, F_NAME);
    sprintf(node_f.addr, F_ADDR);
    node_f.major = F_MAJOR;
    node_f.minor = F_MINOR;
    node_f.x = F_X;
    node_f.y = F_Y;
    sprintf(node_f.left, G_NAME);
    sprintf(node_f.right, E_NAME);
    
    sprintf(node_g.name, G_NAME);
    sprintf(node_g.addr, G_ADDR);
    node_g.major = G_MAJOR;
    node_g.minor = G_MINOR;
    node_g.x = G_X;
    node_g.y = G_Y;
    sprintf(node_g.left, H_NAME);
    sprintf(node_g.right, F_NAME);
    
    sprintf(node_h.name, H_NAME);
    sprintf(node_h.addr, H_ADDR);
    node_h.major = H_MAJOR;
    node_h.minor = H_MINOR;
    node_h.x = H_X;
    node_h.y = H_Y;
    sprintf(node_h.left, A_NAME);
    sprintf(node_h.right, G_NAME);
    
    sprintf(node_i.name, I_NAME);
    sprintf(node_i.addr, I_ADDR);
    node_i.major = I_MAJOR;
    node_i.minor = I_MINOR;
    node_i.x = -1;
    node_i.y = -1;
    sprintf(node_i.left, NA);
    sprintf(node_i.right, NA);
    
    sprintf(node_j.name, J_NAME);
    sprintf(node_j.addr, J_ADDR);
    node_j.major = J_MAJOR;
    node_j.minor = J_MINOR;
    node_j.x = -1;
    node_j.y = -1;
    sprintf(node_j.left, NA);
    sprintf(node_j.right, NA);
    
    sprintf(node_k.name, K_NAME);
    sprintf(node_k.addr, K_ADDR);
    node_k.major = K_MAJOR;
    node_k.minor = K_MINOR;
    node_k.x = -1;
    node_k.y = -1;
    sprintf(node_k.left, NA);
    sprintf(node_k.right, NA);
    
    sprintf(node_l.name, L_NAME);
    sprintf(node_l.addr, L_ADDR);
    node_l.major = L_MAJOR;
    node_l.minor = L_MINOR;
    node_l.x = -1;
    node_l.y = -1;
    sprintf(node_l.left, NA);
    sprintf(node_l.right, NA);
    
    
    // add default static nodes to linked list
    
    sys_slist_append(&static_node_list, &(node_a.node));
    sys_slist_append(&static_node_list, &(node_b.node));
    sys_slist_append(&static_node_list, &(node_c.node));
    sys_slist_append(&static_node_list, &(node_d.node));
    sys_slist_append(&static_node_list, &(node_e.node));
    sys_slist_append(&static_node_list, &(node_f.node));
    sys_slist_append(&static_node_list, &(node_g.node));
    sys_slist_append(&static_node_list, &(node_h.node));
    
    
    // all static nodes: used as reference
    
    sprintf(node_id_a.addr, A_ADDR);
    node_id_a.static_node = &node_a;
    
    sprintf(node_id_b.addr, B_ADDR);
    node_id_b.static_node = &node_b;
    
    sprintf(node_id_c.addr, C_ADDR);
    node_id_c.static_node = &node_c;
    
    sprintf(node_id_d.addr, D_ADDR);
    node_id_d.static_node = &node_d;
    
    sprintf(node_id_e.addr, E_ADDR);
    node_id_e.static_node = &node_e;
    
    sprintf(node_id_f.addr, F_ADDR);
    node_id_f.static_node = &node_f;
    
    sprintf(node_id_g.addr, G_ADDR);
    node_id_g.static_node = &node_g;
    
    sprintf(node_id_h.addr, H_ADDR);
    node_id_h.static_node = &node_h;
    
    sprintf(node_id_i.addr, I_ADDR);
    node_id_i.static_node = &node_i;
    
    sprintf(node_id_j.addr, J_ADDR);
    node_id_j.static_node = &node_j;
    
    sprintf(node_id_k.addr, K_ADDR);
    node_id_k.static_node = &node_k;
    
    sprintf(node_id_l.addr, L_ADDR);
    node_id_l.static_node = &node_l;
    
    
    // add reference to all static nodes to a seperate linked list
    
    sys_slist_append(&all_static_node_list, &(node_id_a.node));
    sys_slist_append(&all_static_node_list, &(node_id_b.node));
    sys_slist_append(&all_static_node_list, &(node_id_c.node));
    sys_slist_append(&all_static_node_list, &(node_id_d.node));
    sys_slist_append(&all_static_node_list, &(node_id_e.node));
    sys_slist_append(&all_static_node_list, &(node_id_f.node));
    sys_slist_append(&all_static_node_list, &(node_id_g.node));
    sys_slist_append(&all_static_node_list, &(node_id_h.node));
    sys_slist_append(&all_static_node_list, &(node_id_i.node));
    sys_slist_append(&all_static_node_list, &(node_id_j.node));
    sys_slist_append(&all_static_node_list, &(node_id_k.node));
    sys_slist_append(&all_static_node_list, &(node_id_l.node));
}


