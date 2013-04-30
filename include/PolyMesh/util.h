namespace util
{
	void CopyMatrix16fv(float * source, float *dest)
	{
		for (int i = 0; i < 16; i++)
		{
			dest[i] = source[i];
		}
	}
}