#pragma once
#include <iostream>
#include <vector>
class ImportantData
{
public:
	ImportantData();

	void SetPostID(const std::string & PostID);
	void SetUsername(const std::string& Username);
	void SetCommentID(const std::string& CommentID);
	void SetOtherUsername(const std::string& OtherUsername);
	void SetCommentUsername(const std::string& CommentUsername);
	void SetText(const std::string& Text);
	void SetTextVector(const std::vector<std::string>& vectorText);

	const std::string& GetPostID() const;
	const std::string& GetUsername() const;
	const std::string& GetCommentID() const;
	const std::string& GetOtherUsername() const;
	const std::string& GetCommentUsername() const;
	const std::vector<std::string>& GetTextVector() const;
	const std::string& GetText() const;

private:
	std::string m_PostID; 
	std::string m_CommentID;
	std::string m_Username;
	std::string m_OtherUsername;
	std::string m_commentUsername;
	std::vector<std::string> m_VectorText;
	std::string m_Text;

};

