//
// ANCMC-001 Firmware (Artificial Neuron Compute Module Connector 001)
// 1.Booting
//  - 1.Initializing RAM (Random-Access-Memory) - Layer 0
//  - 2.Establish communication with host       - Layer 1
//  - 3.Check Modules                           - Layer 2
//  - 4.Request and recieve network             - Layer 3
// 2.Compute network                            - Chase
//  - 1.Load Neuron into compute module
//  - 2.Actualise input for neurons
//  - 3.Check for requests and commands from host
//  - 4.Jump back to 2.1
// 3.Clean up
//  - 1.Send network back to host
//  - 2.Shut communication
//  - 3.Shut power
//
// Protocol
// MSG Code   |Meaning
// 0x0 or Cx0 |Test
// 0x1 or Cx1 |Switch roles (contains: ID)
// 0x2 or Cx2 |Approven
// 0x3 or Cx3 |Denied
// 0x4 or Cx4 |Request data (expects: one approve(0x2 or Cx2) followed by a data block(0x6 or Cx6) contains: msg(what data wanted))
// 0x5 or Cx5 |Get meta info (expects: one approve(0x2 or Cx2) followed by a data block(0x6 or Cx6))
// 0x6 or Cx6 |Data block (contains: size of block in kilobytes, format, encryption key(optional), data)
//
//  Design by Hacklord Captain Bear
//

#define NULL 0x00000000000000000000000000000000
#define GDT_POINTER 0x00000000000000000000000000000001
#define IDT_POINTER 0x00000000000000000000000000000002
#define FREE_MEM_POINTER 0x00000000000000000000000000000003
#define VIDEO_MEM_START 0x00000000000000000000000000000004

// Forward decleration
print_pixel();

void ANCMC_001_Firmware() {}
