#include "inpch.h"

namespace Infinit {

	UUID::UUID()
	{
		fData[0] = (uint8_t)0;
		fData[1] = (uint8_t)0;
		fData[2] = (uint8_t)0;
		fData[3] = (uint8_t)0;
		fData[4] = (uint8_t)0;
		fData[5] = (uint8_t)0;
		fData[6] = (uint8_t)0;
		fData[7] = (uint8_t)0;
		fData[8] = (uint8_t)0;
		fData[9] = (uint8_t)0;
		fData[10] = (uint8_t)0;
		fData[11] = (uint8_t)0;
		fData[12] = (uint8_t)0;
		fData[13] = (uint8_t)0;
		fData[14] = (uint8_t)0;
		fData[15] = (uint8_t)0;
	}
	UUID::UUID(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
	{
		fData[0] = (uint8_t)(a & 0xFF);
		fData[1] = (uint8_t)((a >> 8) & 0xFF);
		fData[2] = (uint8_t)((a >> 16) & 0xFF);
		fData[3] = (uint8_t)((a >> 24) & 0xFF);
		fData[4] = (uint8_t)(b & 0xFF);
		fData[5] = (uint8_t)((b >> 8) & 0xFF);
		fData[6] = (uint8_t)((b >> 16) & 0xFF);
		fData[7] = (uint8_t)((b >> 24) & 0xFF);
		fData[8] = (uint8_t)(c & 0xFF);
		fData[9] = (uint8_t)((c >> 8) & 0xFF);
		fData[10] = (uint8_t)((c >> 16) & 0xFF);
		fData[11] = (uint8_t)((c >> 24) & 0xFF);
		fData[12] = (uint8_t)(d & 0xFF);
		fData[13] = (uint8_t)((d >> 8) & 0xFF);
		fData[14] = (uint8_t)((d >> 16) & 0xFF);
		fData[15] = (uint8_t)((d >> 24) & 0xFF);
	}

	UUID::UUID(const UUID& src)
	{
		for (int i = 0; i < 16; i++)
			fData[i] = src.fData[i];
	}

	UUID::~UUID()
	{

	}

	UUID& UUID::operator=(const UUID& src)
	{
		for (int i = 0; i < 16; i++)
			fData[i] = src.fData[i];
		return *this;
	}

	bool UUID::operator==(const UUID& id) const
	{
		for (int i = 0; i < 16; i++) {
			if (fData[i] != id.fData[i])
				return false;
		}
		return true;
	}

	bool UUID::operator!=(const UUID& id) const
	{
		return !operator==(id);
	}


	const UUID& UUID::CreateNew()
	{
		GUID	guid;
		UuidCreateSequential(&guid);


		fData[0] = (guid.Data1 & 0xff000000) >> 24;
		fData[1] = (guid.Data1 & 0x00ff0000) >> 16;
		fData[2] = (guid.Data1 & 0x0000ff00) >> 8;
		fData[3] = (guid.Data1 & 0x000000ff);

		fData[4] = (guid.Data2 & 0xff00) >> 8;
		fData[5] = (guid.Data2 & 0x00ff);


		fData[6] = (guid.Data3 & 0xff00) >> 8;
		fData[7] = (guid.Data3 & 0x00ff);


		fData[8] = guid.Data4[0];
		fData[9] = guid.Data4[1];
		fData[10] = guid.Data4[2];
		fData[11] = guid.Data4[3];
		fData[12] = guid.Data4[4];
		fData[13] = guid.Data4[5];
		fData[14] = guid.Data4[6];
		fData[14] = guid.Data4[7];

		return *this;
	}

	void UUID::GetUUID(uint32_t& out1, uint32_t& out2, uint32_t& out3, uint32_t& out4) const
	{
		out1 = (uint32_t)(fData[0] + fData[1] * 256 + fData[2] * 65536 + fData[3] * 16777216);
		out2 = (uint32_t)(fData[4] + fData[5] * 256 + fData[6] * 65536 + fData[7] * 16777216);
		out3 = (uint32_t)(fData[8] + fData[9] * 256 + fData[10] * 65536 + fData[11] * 16777216);
		out4 = (uint32_t)(fData[12] + fData[13] * 256 + fData[14] * 65536 + fData[15] * 16777216);
	}

	bool UUID::operator < (const UUID& iid) const
	{
		bool	isLess = true;
		bool	isEqual = true;

		uint32_t a, b, c, d;
		GetUUID(a, b, c, d);

		uint32_t iida, iidb, iidc, iidd;
		iid.GetUUID(iida, iidb, iidc, iidd);

		// a
		if (isEqual)
		{
			isEqual = isEqual && a == iida;
			if (!isEqual) { isLess = (a < iida); }

			// b
			if (isEqual)
			{
				isEqual = isEqual && c == iidb;
				if (!isEqual) { isLess = (c < iidc); }

				// c
				if (isEqual)
				{
					isEqual = isEqual && c == iidc;
					if (!isEqual) { isLess = (c < iidc); }

					// d
					if (isEqual)
					{
						isEqual = isEqual && d == iidd;
						if (!isEqual) { isLess = (d < iidd); }
					} //d
				} //c
			} //b
		} //a

		return isLess && !isEqual;
	};
	static char GetPieceAsChar(uint8_t piece)
	{
		IN_CORE_ASSERT(piece <= 0x0F, "Failed to get piece of uuid");
		char	ch;
		if (piece <= 9)
			ch = '0' + piece;
		else
			ch = 'A' + (piece - 0x0A);

		return ch;
	}

	static string GetPieceAsText(uint8_t piece)
	{
		char	buffer[3];
		buffer[0] = GetPieceAsChar(piece >> 4);
		buffer[1] = GetPieceAsChar(piece & 0x0F);
		buffer[2] = 0x00;

		string str = buffer;
		return str;
	}

	string UUID::ToString() const
	{
		string str;
		str += "{";
		str += GetPieceAsText(fData[0]);
		str += GetPieceAsText(fData[1]);
		str += GetPieceAsText(fData[2]);
		str += GetPieceAsText(fData[3]);
		str += "-";
		str += GetPieceAsText(fData[4]);
		str += GetPieceAsText(fData[5]);
		str += "-";
		str += GetPieceAsText(fData[6]);
		str += GetPieceAsText(fData[7]);
		str += "-";
		str += GetPieceAsText(fData[8]);
		str += GetPieceAsText(fData[9]);
		str += "-";
		str += GetPieceAsText(fData[10]);
		str += GetPieceAsText(fData[11]);
		str += GetPieceAsText(fData[12]);
		str += GetPieceAsText(fData[13]);
		str += GetPieceAsText(fData[14]);
		str += GetPieceAsText(fData[15]);
		str += "}";
		return str;
	}

	static uint8_t GetDigitForChar(char ch)
	{
		if (ch >= '0' && ch <= '9') { return ch - '0'; }
		if (ch >= 'A' && ch <= 'F') { return 0x0A + (ch - 'A'); }
		if (ch >= 'a' && ch <= 'f') { return 0x0A + (ch - 'a'); }

		return 0;
	}

	static uint8_t GetDigitForPiece(const char* pString)
	{
		uint8_t a = GetDigitForChar(*(pString + 0));
		uint8_t b = GetDigitForChar(*(pString + 1));

		uint8_t res = a << 4 | (b & 0x0F);
		return res;
	}

	bool UUID::FromString(const string& str)
	{
		// {09E95D97-364C-43d5-8ADF-FF4CE0EC41A7}
		bool b = str.length() == 38;
		if (b)
		{
			bool	b1 = str[0] == '{';
			bool	b2 = str[9] == '-';
			bool	b3 = str[14] == '-';
			bool	b4 = str[19] == '-';
			bool	b5 = str[24] == '-';
			bool	b6 = str[37] == '}';

			b = b1 && b2 && b3 && b4 && b5 && b6;
		}

		if (!b)
		{
			// create new UUID if this is not a valid UUID
			UUID newUUID;
			*this = newUUID;
		}

		if (b)
		{
			const char* pString = str.c_str();

			fData[0] = GetDigitForPiece(pString + 1);
			fData[1] = GetDigitForPiece(pString + 3);
			fData[2] = GetDigitForPiece(pString + 5);
			fData[3] = GetDigitForPiece(pString + 7);
			fData[4] = GetDigitForPiece(pString + 10);
			fData[5] = GetDigitForPiece(pString + 12);
			fData[6] = GetDigitForPiece(pString + 15);
			fData[7] = GetDigitForPiece(pString + 17);
			fData[8] = GetDigitForPiece(pString + 20);
			fData[9] = GetDigitForPiece(pString + 22);
			fData[10] = GetDigitForPiece(pString + 25);
			fData[11] = GetDigitForPiece(pString + 27);
			fData[12] = GetDigitForPiece(pString + 29);
			fData[13] = GetDigitForPiece(pString + 31);
			fData[14] = GetDigitForPiece(pString + 33);
			fData[15] = GetDigitForPiece(pString + 35);
		}

		return b;
	}

	bool UUID::IsValid() const
	{
		bool valid = false;
		for (int i = 0; i < 16; i++)
		{
			if (fData[i] != 0)
			{
				valid = true;
			}
		}
		return valid;
	}

	const uint8_t* UUID::GetData() const
	{
		return &*fData;
	}

	uint8_t* UUID::GetData()
	{
		return &*fData;
	}


}