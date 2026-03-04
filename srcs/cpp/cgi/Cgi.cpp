#include "../../hpp/Client.hpp"

typedef std::vector<char *>	argvVector;

void		get_argv(Client &client, argvVector &v);
void		run_cmd(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv);
void		run_daemon(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv);

std::string		createHtmlPokedex(s_cgi &cgi);
std::string		createHtmlCub(t_cgi &cgi_data, Server &srv, Client &client);
std::string		createHtmlYouTube(t_cgi &cgi_data);
std::string		createHtmlCrypter(t_cgi &cgi_ptr);
void			createArgvCrypter(std::string &args, argvVector &argv);
void			createArgvWeaksleep(std::string &args, argvVector &argv_data);
std::string		createHtmlWeaksleep(t_cgi &cgi_data);

//FIXME - 
void	run_script(Server &srv, Client &client, std::string &body)
{
	t_cgi				cgi_data(client);
	t_cgi				*cgi_ptr = NULL;
	std::vector<char *> argv;

	std::cout << "RUN_SCRIPT\n";
	if (srv.getFdData()[client.getSockFd()].cgi_ready == false)//prima volta
	{
		get_argv(client, argv);
		if (client.getLocConf().fastcgi_bool == true)
			run_daemon(srv, client, cgi_data, argv);
		else
			run_cmd(srv, client, cgi_data, argv);
		return ;
	}
	else // se cgi gia eseguita, ripesca dati cgi
		cgi_ptr = srv.getFdData()[client.getSockFd()].cgi;
	client.getRequest().setBodyType("text/html");
	if (client.getLocConf().script_type == "pokedex")
		body = createHtmlPokedex(*cgi_ptr);
	else if (client.getLocConf().script_type == "cub3D")
		body = createHtmlCub(*cgi_ptr, srv, client);
	else if (client.getLocConf().script_type == "giorgio")
		body = createHtmlYouTube(*cgi_ptr);
	else if (client.getLocConf().script_type == "crypter")
		body = createHtmlCrypter(*cgi_ptr);
	else if (client.getLocConf().script_type == "weaksleep")
		body = createHtmlWeaksleep(*cgi_ptr);
	else
	{
		body = cgi_ptr->output;
		client.getRequest().setBodyType("text/plain");
		std::cout << "script_type undefined. no html created." << std::endl;
	}
	if (client.getLocConf().fastcgi_bool == false)
		delete cgi_ptr;
	else
		cgi_ptr->reset();
	srv.getFdData()[client.getSockFd()].cgi = NULL;
	srv.getFdData()[client.getSockFd()].cgi_ready = false;
}

s_cgi::s_cgi(void)
{
	this->isFastCgiBool = false;
	this->pid = 0;
	this->pipe[0] = 0;
	this->pipe[1] = 0;
	this->poll_index[0] = 0;
	this->poll_index[1] = 0;
	this->bytes_read = 0;
	this->output_len = 0;
	this->isParsed = false;
}

s_cgi::s_cgi(Client &client)
{
	this->bytes_read = 0;
	this->pipe[0] = 0;
	this->pipe[1] = 0;
	this->poll_index[0] = 0;
	this->poll_index[1] = 0;
	this->pid = 0;
	this->output_len = 0;
	this->isParsed = false;
	if (client.getLocConf().exist == true && client.getLocConf().fastcgi_bool == true)
		this->isFastCgiBool = true;
	else
		this->isFastCgiBool = false;
}

s_cgi::s_cgi(const s_cgi &other)
{
	*this = other;
}

s_cgi	&s_cgi::operator=(const s_cgi &other)
{
	if (this == &other)
		return (*this);
	this->poll_index[0] = other.poll_index[0];
	this->poll_index[1] = other.poll_index[1];
	this->pid = other.pid;
	this->pipe[0] = other.pipe[0];
	this->pipe[1] = other.pipe[1];
	this->output = other.output;
	this->output_len = other.output_len;
	this->bytes_read = other.bytes_read;
	this->isFastCgiBool = other.isFastCgiBool;
	this->isParsed = other.isParsed;
	return (*this);
}

// return -1 if an error occurs, else 0
int		s_cgi::headerParsing(Client &client)
{
	int			bytes;
	std::string	output;

	client.getBuffer().resize(CGI_HEADER_LEN);
	bytes = read(this->pipe[0], client.getBuffer().data() + this->bytes_read, CGI_HEADER_LEN - this->bytes_read);
	if (bytes <= 0)
		return (std::cerr << "readCgiError: cgi is gone\n", -1);
	this->bytes_read += bytes;
	if (this->bytes_read != CGI_HEADER_LEN)
		return (0);
	client.getBuffer().push_back('\0');
	output = client.getBuffer().data();
	client.getBuffer().clear();
	this->bytes_read = 0;
	if (output.compare(0, 3, "OK|") != 0 || output.rbegin()[0] != '|')
		return (std::cerr << "readCgiErrorFormat: " << output << "\n", -1);
	bytes = std::atoi(output.c_str() + 3);
	if (bytes <= 0)
		return (std::cerr << "readCgiErrorSize: " << output << "\n", -1);
	this->output_len = bytes;
	this->isParsed = true;
	return (0);
}

int		s_cgi::readChunk(Client &client)
{
	int	bytes;

	client.getBuffer().resize(this->bytes_read + CHUNK_READ);
	bytes = read(this->pipe[0], client.getBuffer().data() + this->bytes_read, CHUNK_READ);
	if (bytes <= 0)
		return (-1);
	this->bytes_read += bytes;
	client.getBuffer().resize(this->bytes_read);
	//std::cout << "s_cgi::readChunk(): bytes read: " << this->bytes_read << "/";
	//std::cout << this->output_len << "\n";
	return (bytes);
}

void	s_cgi::removeFromPoll(bool is_pipe_out, Server &srv)
{
	int			fd_last;
	int			poll_index;

	poll_index = this->poll_index[is_pipe_out];
	if (!poll_index)
		return ;
	fd_last = srv.getAddrsVector().back().fd;
	if (srv.getFdData()[fd_last].cgi)
	{
		srv.getFdData()[fd_last].cgi->poll_index[0] = this->poll_index[0];
		srv.getFdData()[fd_last].cgi->poll_index[1] = this->poll_index[1];
	}
	if ((size_t)poll_index < srv.getAddrSize())
		std::swap(srv.getAddrsVector()[poll_index], srv.getAddrsVector().back());
	srv.getAddrsVector().pop_back();
	std::cout << "s_cgi::removeFromPoll() pipe fd: " << this->pipe[0] << "\n";
	close_fd(&this->pipe[is_pipe_out]);
}

void	s_cgi::clear(Server &srv, Client &client)
{
	if (this->pid != 0 && this->isFastCgiBool == true)
		kill(this->pid, SIGKILL);
	this->pid = 0;
	if (this->pipe[0])
		this->removeFromPoll(false, srv);
	if (this->pipe[1])
		this->removeFromPoll(true, srv);
	if (client.getCookieData().exist == true)
		srv.getCookieMap()[client.getCookieData().id].cgi = NULL;
	client.getCookieData().cgi = NULL;
}

void	s_cgi::clear()
{
	if (this->pid != 0 && this->isFastCgiBool == true)
		kill(this->pid, SIGKILL);
	this->pid = 0;
	close_fd(&this->pipe[0]);
	close_fd(&this->pipe[1]);
}

void	s_cgi::reset()
{
	this->bytes_read = 0;
	this->isParsed = false;
	this->input.clear();
	this->output = NULL;
	this->output_len = 0;
}