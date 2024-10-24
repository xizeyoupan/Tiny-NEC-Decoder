# Tiny NEC Decoder for Embedded C

The NEC IR transmission protocol uses pulse distance encoding of the message bits. This repository is designed to decode NEC protocol with flexible configuration on most embedded devices.

## Requirements
- A 100-μs timer for continuous scanning.
- Less than 10 bytes of RAM and less than 100 bytes of ROM with minimal configuration.

## Feature
- Easy to use.
- Not focused on any specific device, compatible with cheap **Chinese** 8-bit chips.
- Various configuration options and callback functions.
- Repeat codes supported.

