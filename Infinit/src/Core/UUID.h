#pragma once

namespace Infinit {

	class UUID
	{
	public:
		UUID();
		// the bits are reversed
		//	for example:
		//	{09E95D97-364C-43d5-8ADF-FF4CE0EC41A7}
		//	'975DE909' 'd5434C36' '4CFFDF8A' 'A741ECE0'
		UUID(uint32_t  a, uint32_t  b, uint32_t  c, uint32_t  d);
		// copy the bytes one by one
		UUID(const UUID& src);
		~UUID();

		UUID& operator=(const UUID& src);
		bool		operator==(const UUID& id) const;
		bool		operator!=(const UUID& id) const;
		bool    	operator < (const UUID& iid) const;

		string			ToString() const;
		bool			FromString(const string& id);


		// create four 32 Bit integer from the components of the GUID structure
		// the bits are reversed
		//	for example:
		//	{09E95D97-364C-43d5-8ADF-FF4CE0EC41A7}
		//	'975DE909' 'd5434C36' '4CFFDF8A' 'A741ECE0'
		void			GetUUID(uint32_t& out1, uint32_t& out2, uint32_t& out3, uint32_t& out4) const;
		const UUID& CreateNew();
		bool 			IsValid() const;
		const uint8_t* GetData() const;
		uint8_t* GetData();
	protected:
		// {09E95D97-364C-43d5-8ADF-FF4CE0EC41A7}
		uint8_t		fData[16];
	};

}