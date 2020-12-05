
class FileException : public std::exception
{
private:
	std::string m_error;

public:
	FileException(std::string error)
		: m_error(error)
	{
	}
	const char* what() const noexcept { return m_error.c_str(); }
};