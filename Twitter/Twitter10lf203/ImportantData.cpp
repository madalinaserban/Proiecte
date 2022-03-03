#include "ImportantData.h"

ImportantData::ImportantData() = default;

void ImportantData::SetPostID(const std::string& PostID)
{
	this->m_PostID = PostID;
}

void ImportantData::SetUsername(const std::string& Username)
{
	this->m_Username = Username;
}

void ImportantData::SetCommentID(const std::string& CommentID)
{
	this->m_CommentID = CommentID;
}

void ImportantData::SetOtherUsername(const std::string& OtherUsername)
{
	this->m_OtherUsername = OtherUsername;
}

void ImportantData::SetCommentUsername(const std::string& CommentUsername)
{
	this->m_commentUsername = CommentUsername;
}

void ImportantData::SetText(const std::string& Text)
{
	this->m_Text = Text;
}

void ImportantData::SetTextVector(const std::vector<std::string>& vectorText)
{
	this->m_VectorText = vectorText;
}

const std::string& ImportantData::GetPostID() const
{
	return this->m_PostID;
}

const std::string& ImportantData::GetUsername() const
{
	return this->m_Username;
}

const std::string& ImportantData::GetCommentID() const
{
	return this->m_CommentID;
}

const std::string& ImportantData::GetOtherUsername() const
{
	return this->m_OtherUsername;
}

const std::string& ImportantData::GetCommentUsername() const
{
	return this->m_commentUsername;
}

const std::vector<std::string>& ImportantData::GetTextVector() const
{
	return this->m_VectorText;
}

const std::string& ImportantData::GetText() const
{
	return this->m_Text;
}
