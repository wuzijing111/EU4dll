﻿#include "pch.h"
#include "plugin_64.h"
#include "escape_tool.h"

namespace FileSave {
	extern "C" {
		void fileSaveProc1();
		void fileSaveProc2();
		void fileSaveProc3();
		void fileSaveProc3V130();
		void fileSaveProc4();
		void fileSaveProc5();
		void fileSaveProc5V130();
		void fileSaveProc6();
		void fileSaveProc6V130();
		void fileSaveProc7();
		uintptr_t fileSaveProc1ReturnAddress;
		uintptr_t fileSaveProc2ReturnAddress;
		uintptr_t fileSaveProc2CallAddress;
		uintptr_t fileSaveProc3ReturnAddress;
		uintptr_t fileSaveProc3CallAddress;
		uintptr_t fileSaveProc4ReturnAddress;
		uintptr_t fileSaveProc4CallAddress;
		uintptr_t fileSaveProc4MarkerAddress;
		uintptr_t fileSaveProc5ReturnAddress;
		uintptr_t fileSaveProc5CallAddress;
		uintptr_t fileSaveProc5MarkerAddress;
		uintptr_t fileSaveProc6ReturnAddress;
		uintptr_t fileSaveProc6CallAddress;
		uintptr_t fileSaveProc6MarkerAddress;
		uintptr_t fileSaveProc7ReturnAddress;
		uintptr_t fileSaveProc7CallAddress;
	}
	

	DllError fileSaveProc1Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v1_29_2_0:
		case v1_29_3_0:
		case v1_29_4_0:
		case v1_30_1_0:
		case v1_30_2_0:
		case v1_30_3_0:
		case v1_30_4_0:
		case v1_30_5_0:
		case v1_31_1_0:
		case v1_31_2_0:
		case v1_31_3_0:
		case v1_31_4_0:
			// mov     eax, [rcx+10h]
			BytePattern::temp_instance().find_pattern("8B 41 10 85 C0 0F 84 31 01 00 00");
			if (BytePattern::temp_instance().has_size(1, u8"ファイル名を安全にしている場所を短絡する")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				fileSaveProc1ReturnAddress = Injector::GetBranchDestination(address + 0x5).as_int();

				Injector::MakeJMP(address, fileSaveProc1, true);
			}
			else {
				e.unmatch.fileSaveProc1Injector = true;
			}
			break;
		default:
			e.version.fileSaveProc1Injector = true;
		}

		return e;
	}

	DllError fileSaveProc2Injector(RunOptions options) {
		DllError e = {};
		std::string pattern;

		switch (options.version) {
		case v1_31_4_0:
		case v1_31_3_0:
		case v1_31_2_0:
		case v1_31_1_0:
			pattern = "48 8D 05 ? ? A7 FF 48 3B D0 75 06 48 8D 41 30";
			goto TAG;
		case v1_30_5_0:
			pattern = "48 8D 05 51 D1 B3 FF 48 3B D0 75 06 48 8D 41 30";
			goto TAG;
		case v1_30_4_0:
			pattern = "48 8D 05 ? ? B4 FF 48 3B D0 75 06 48 8D 41 30";
			goto TAG;
		case v1_30_3_0:
		case v1_30_2_0:
			pattern = "48 8D 05 B1 4B B4 FF 48 3B D0 75 06 48 8D 41 30";
			goto TAG;
		case v1_30_1_0:
			pattern = "48 8D 05 91 4E B4 FF 48 3B D0 75 06 48 8D 41 30";
			goto TAG;
		case v1_29_4_0:
			pattern = "48 8D 05 91 FB A4 FF 48 3B D0 75 06 48 8D 41 30";
			goto TAG;
		case v1_29_3_0:
			pattern = "48 8D 05 11 92 A5 FF 48 3B D0 75 06 48 8D 41 30";
			goto TAG;
		case v1_29_2_0:
			// lea     rax, sub_xxxxx ここしか取れなかった...
			pattern = "48 8D 05 01 A9 A5 FF 48 3B D0 75 06 48 8D 41 30";
		TAG:
			BytePattern::temp_instance().find_pattern(pattern);
			if (BytePattern::temp_instance().has_size(1, u8"ファイル名をUTF-8に変換して保存できるようにする")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				fileSaveProc2CallAddress = (uintptr_t) escapedStrToUtf8;

				// jnz     short loc_xxxxx
				fileSaveProc2ReturnAddress = address + 0x14 + 0x1B;

				// cmp word ptr [rax+18h], 10h
				Injector::MakeJMP(address + 0x14, fileSaveProc2, true);
			}
			else {
				e.unmatch.fileSaveProc2Injector = true;
			}
			break;
		default:
			e.version.fileSaveProc2Injector = true;
		}

		return e;
	}

	DllError fileSaveProc3Injector(RunOptions options) {
		DllError e = {};
		switch (options.version) {
		case v1_29_3_0:
		case v1_29_2_0:
		case v1_29_4_0:
			//  jmp     short loc_xxxxx
			BytePattern::temp_instance().find_pattern("EB 6E 48 8D 15 ? ? ? ? FF 90 98 00 00 00 48");
			if (BytePattern::temp_instance().has_size(1, u8"ダイアログでのセーブエントリのタイトルを表示できるようにする")) {
				//  lea     rdx, aSave_game_titl ; "save_game_title"
				uintptr_t address = BytePattern::temp_instance().get_first().address() + 0x2;

				fileSaveProc3CallAddress = (uintptr_t)utf8ToEscapedStr;

				// call sub_xxxxx
				fileSaveProc3ReturnAddress = address + 0x1A;

				Injector::MakeJMP(address, fileSaveProc3, true);
			}
			else {
				e.unmatch.fileSaveProc3Injector = true;
			}
			break;
		case v1_30_5_0:
		case v1_30_4_0:
		case v1_30_3_0:
		case v1_30_2_0:
		case v1_30_1_0:
		case v1_31_1_0:
		case v1_31_2_0:
		case v1_31_3_0:
		case v1_31_4_0:
			//  jmp     short loc_xxxxx
			BytePattern::temp_instance().find_pattern("EB 6E 48 8D 15 ? ? ? ? FF 90 98 00 00 00 48");
			if (BytePattern::temp_instance().has_size(1, u8"ダイアログでのセーブエントリのタイトルを表示できるようにする")) {
				//  lea     rdx, aSave_game_titl ; "save_game_title"
				uintptr_t address = BytePattern::temp_instance().get_first().address() + 0x2;

				fileSaveProc3CallAddress = (uintptr_t)utf8ToEscapedStr;

				// call sub_xxxxx
				fileSaveProc3ReturnAddress = address + 0x1A;

				Injector::MakeJMP(address, fileSaveProc3V130, true);
			}
			else {
				e.unmatch.fileSaveProc3Injector = true;
			}
			break;
		default:
			e.version.fileSaveProc3Injector = true;
		}

		return e;
	}

	DllError fileSaveProc4Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v1_29_2_0:
		case v1_29_3_0:
		case v1_29_4_0:
		case v1_30_1_0:
		case v1_30_2_0:
		case v1_30_3_0:
		case v1_30_4_0:
		case v1_30_5_0:
		case v1_31_1_0:
		case v1_31_2_0:
		case v1_31_3_0:
		case v1_31_4_0:
			// lea     r8, [rbp+0]
			BytePattern::temp_instance().find_pattern("4C 8D 45 00 48 8D 15 ? ? ? ? 48 8D 4C 24 70 E8 ? ? ? ? 90");
			if (BytePattern::temp_instance().has_size(1, u8"ダイアログでのセーブエントリのツールチップを表示できるようにする1")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				fileSaveProc4CallAddress = (uintptr_t)utf8ToEscapedStr2;

				// lea rdx, {aZy}
				fileSaveProc4MarkerAddress = Injector::GetBranchDestination(address + 4).as_int();

				// call sub_xxxxx
				fileSaveProc4ReturnAddress = address + 0x10;

				Injector::MakeJMP(address, fileSaveProc4, true);
			}
			else {
				e.unmatch.fileSaveProc4Injector = true;
			}
			break;
		default:
			e.version.fileSaveProc4Injector = true;
		}

		return e;
	}

	DllError fileSaveProc5Injector(RunOptions options) {
		DllError e = {};
		std::string pattern;

		switch (options.version) {
		case v1_29_2_0:
		case v1_29_3_0:
		case v1_29_4_0:
			// lea     r8, [r14+598h]
			BytePattern::temp_instance().find_pattern("4D 8D 86 98 05 00 00 48 8D 15 ? ? ? ? 48 8D 4C 24 50");
			if (BytePattern::temp_instance().has_size(1, u8"ダイアログでのセーブエントリのツールチップを表示できるようにする2")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				fileSaveProc5CallAddress = (uintptr_t)utf8ToEscapedStr2;

				// lea rdx, {aZy}
				fileSaveProc5MarkerAddress = Injector::GetBranchDestination(address + 7).as_int();

				// call sub_xxxxx
				fileSaveProc5ReturnAddress = address + 0x13;

				Injector::MakeJMP(address, fileSaveProc5, true);
			}
			else {
				e.unmatch.fileSaveProc5Injector = true;
			}
			break;
		case v1_30_5_0:
		case v1_30_4_0:
		case v1_30_3_0:
		case v1_30_2_0:
		case v1_30_1_0:
		case v1_31_1_0:
		case v1_31_2_0:
		case v1_31_3_0:
		case v1_31_4_0:
			// lea     r8, [r14+5C0h]
			BytePattern::temp_instance().find_pattern("4D 8D 86 C0 05 00 00 48 8D 15 ? ? ? ? 48 8D 4C 24 50");
			if (BytePattern::temp_instance().has_size(1, u8"ダイアログでのセーブエントリのツールチップを表示できるようにする2")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				fileSaveProc5CallAddress = (uintptr_t)utf8ToEscapedStr2;

				// lea rdx, {aZy}
				fileSaveProc5MarkerAddress = Injector::GetBranchDestination(address + 7).as_int();

				// call sub_xxxxx
				fileSaveProc5ReturnAddress = address + 0x13;

				Injector::MakeJMP(address, fileSaveProc5V130, true);
			}
			else {
				e.unmatch.fileSaveProc5Injector = true;
			}
			break;
		default:
			e.version.fileSaveProc5Injector = true;
		}

		return e;
	}

	DllError fileSaveProc6Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v1_29_3_0:
		case v1_29_4_0:
			// lea     r8, [rbp+380h]
			BytePattern::temp_instance().find_pattern("4C 8D 85 80 03 00 00 48 8D 15 ? ? ? ? 48 8D 4C 24 30");
			if (BytePattern::temp_instance().has_size(1, u8"スタート画面でのコンティニューのツールチップ")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				fileSaveProc6CallAddress = (uintptr_t)utf8ToEscapedStr2;

				// lea r8, {aZy}
				fileSaveProc6MarkerAddress = Injector::GetBranchDestination(address + 7).as_int();

				// call sub_xxxxx
				fileSaveProc6ReturnAddress = address + 0x13;

				Injector::MakeJMP(address, fileSaveProc6, true);
			}
			else {
				e.unmatch.fileSaveProc6Injector = true;
			}
			break;
		case v1_30_5_0:
		case v1_30_4_0:
		case v1_30_3_0:
		case v1_30_2_0:
		case v1_30_1_0:
		case v1_31_1_0:
		case v1_31_2_0:
		case v1_31_3_0:
		case v1_31_4_0:
			// lea     r8, [rbp+730h+var_3A0]
			BytePattern::temp_instance().find_pattern("4C 8D 85 90 03 00 00 48 8D 15 ? ? ? ? 48 8D 4C 24 30");
			if (BytePattern::temp_instance().has_size(1, u8"スタート画面でのコンティニューのツールチップ")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				fileSaveProc6CallAddress = (uintptr_t)utf8ToEscapedStr2;

				// lea r8, {aZy}
				fileSaveProc6MarkerAddress = Injector::GetBranchDestination(address + 7).as_int();

				// call sub_xxxxx
				fileSaveProc6ReturnAddress = address + 0x13;

				Injector::MakeJMP(address, fileSaveProc6V130, true);
			}
			else {
				e.unmatch.fileSaveProc6Injector = true;
			}
			break;
		default:
			e.version.fileSaveProc6Injector = true;
		}

		return e;
	}

	DllError fileSaveProc7Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v1_29_3_0:
		case v1_29_4_0:
			// lea     rcx, [rbx+0C8h]
			BytePattern::temp_instance().find_pattern("48 8D 8B C8 00 00 00 48 8B 01 48 8D 54 24 28");
			if (BytePattern::temp_instance().has_size(1, u8"セーブダイアログでのインプットテキストエリア")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				fileSaveProc7CallAddress = (uintptr_t)utf8ToEscapedStr2;

				// call    qword ptr [rax+80h]
				fileSaveProc7ReturnAddress = address + 0xF;

				Injector::MakeJMP(address, fileSaveProc7, true);
			}
			else {
				e.unmatch.fileSaveProc7Injector = true;
			}
			break;
		case v1_30_5_0:
		case v1_30_4_0:
		case v1_30_3_0:
		case v1_30_2_0:
		case v1_30_1_0:
		case v1_31_1_0:
		case v1_31_2_0:
		case v1_31_3_0:
		case v1_31_4_0:
			// lea     rcx, [rbx+0C8h]
			BytePattern::temp_instance().find_pattern("48 8D 8B C8 00 00 00 48 8B 01 48 8D 54 24 28");
			if (BytePattern::temp_instance().has_size(2, u8"セーブダイアログでのインプットテキストエリア")) {
				uintptr_t address = BytePattern::temp_instance().get_second().address();

				fileSaveProc7CallAddress = (uintptr_t)utf8ToEscapedStr2;

				// call    qword ptr [rax+80h]
				fileSaveProc7ReturnAddress = address + 0xF;

				Injector::MakeJMP(address, fileSaveProc7, true);
			}
			else {
				e.unmatch.fileSaveProc7Injector = true;
			}
			break;
		default:
			e.version.fileSaveProc7Injector = true;
		}

		return e;
	}

	DllError Init(RunOptions options) {
		DllError result = {};

		/* UTF-8ファイルを列挙できない問題は解決された */
		result |= fileSaveProc1Injector(options);
		result |= fileSaveProc2Injector(options);
		result |= fileSaveProc3Injector(options);
		// これは使われなくなった？
		//result |= fileSaveProc4Injector(options);
		result |= fileSaveProc5Injector(options);
		result |= fileSaveProc6Injector(options);
		result |= fileSaveProc7Injector(options);

		return result;
	}
}
