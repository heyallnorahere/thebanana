#pragma once
void init_decoders();
short* load_sound(const std::string& path, size_t& samples, unsigned char& channels, unsigned int& samples_per_second);