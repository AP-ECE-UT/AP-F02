#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string NOT_ENOUGH_MONEY = "Not enough money";
const std::string DEPOSIT_CREATED = "OK";
const std::string PASS_TIME_SUCCESS = "OK";
const std::string CREATE_SHORT_TERM_CMD = "create_short_term_deposit";
const std::string CREATE_LONG_TERM_CMD = "create_long_term_deposit";
const std::string CREATE_GHARZOL_CMD = "create_gharzolhasane_deposit";
const std::string CALC_MONEY_IN_BANK_CMD = "calc_money_in_bank";
const std::string CALC_ALL_MONEY_CMD = "calc_all_money";
const std::string PASS_TIME_CMD = "pass_time";
const std::string INVALID_CMD = "invalid command.";
const std::string BANKS_FILE_PATH = "Banks.csv";
const std::string USERS_FILE_PATH = "Users.csv";
const std::string OPEN_FILE_FAILED = "failed to open the file";
const std::string NOT_SUITABLE_PARAMS = "too many or not enough parameters in one of .csv files";
const std::string BANK_ARG_SIGN = "-b";
const std::string USER_ARG_SIGN = "-u";
const std::string NOT_FOUND_ARG = "Input arguments not found.";
const std::string INVALID_SHORT_TERM_DEPOSIT = "Invalid short-term deposit";
const std::string INVENTORY_REPORT_CMD = "inventory_report";
const int PRECISION = 2;
const int BANK_SIGNUP_PARAMS = 3;
const int USER_SIGNUP_PARAMS = 2;
const int INITIAL_AVAILABLE_ID = 1;
const int DEFAULT_ID = 0;
const char CSV_WORD_PARSER = ',';

using uint = unsigned int;
using ushort = unsigned short;

using RequestResult = std::string;

enum DepositType {
    SHORT_TERM,
    LONG_TERM,
    GHARZOL,
};

class User {
public:
    User(uint id, double wallet);
    uint get_id();
    double get_wallet();
    void decrease_wallet(double money);

private:
    uint id;
    double wallet;
};

class Deposit {
public:
    Deposit(uint id, uint user_id, double init_investment, DepositType type, uint years = 0, Deposit* short_term_link = nullptr);
    uint get_user_id();
    uint get_id();
    double get_cur_investment();
    DepositType get_type();
    void apply_interest(ushort short_term_profit_margin);
    void add_money(double money);

private:
    uint id;
    uint user_id;
    uint years;
    double init_investment;
    double cur_investment;
    DepositType type;
    Deposit* short_term_link;
    void apply_short_term_interest(ushort short_term_profit_margin);
    void apply_long_term_interest(ushort short_term_profit_margin);
    void apply_gharzol_interest();
};

class Bank {
public:
    Bank(uint id, ushort short_term_profit_margin, double short_term_min_investment);
    uint get_id();
    RequestResult create_deposit(uint user_id, double init_investment, DepositType type, uint years = 0, uint short_term_link_id = 0);
    RequestResult get_inventory_report(uint user_id, uint short_term_id);
    double calc_user_money(uint user_id);
    void pass_month();

private:
    uint id;
    uint available_deposit_id;
    ushort short_term_profit_margin;
    double short_term_min_investment;
    double donate;
    std::vector<Deposit*> deposits;
    Deposit* find_deposit_by_id(uint id);
};

class CentralBank {
public:
    CentralBank(std::vector<Bank*>, std::vector<User*>);
    RequestResult handle_create_deposit(uint user_id, uint bank_id, double init_investment, DepositType type, uint years = 0, uint short_term_link_id = 0);
    RequestResult handle_calc_money_in_bank(uint user_id, uint bank_id);
    RequestResult handle_calc_all_money(uint user_id);
    RequestResult handle_pass_time(uint num_of_months);
    RequestResult handle_inventory_report(uint user_id, uint bank_id, uint short_term_id);

private:
    std::vector<Bank*> banks;
    std::vector<User*> users;
    Bank* find_bank_by_id(uint id);
    User* find_user_by_id(uint id);
    void pass_month();
};

class CSVHandler {
public:
    CSVHandler(std::string users_file_path, std::string banks_file_path);
    std::vector<User*> read_users_file();
    std::vector<Bank*> read_banks_file();

private:
    std::string users_file_path;
    std::string banks_file_path;
    std::vector<Bank*> parse_and_assign_banks(std::ifstream& file);
    std::vector<User*> parse_and_assign_users(std::ifstream& file);
    std::vector<std::string> parse_line(std::string line);
    Bank* signup_bank(std::vector<std::string>);
    User* signup_user(std::vector<std::string>);
};

class IOHandler {
public:
    IOHandler(int argc, char* argv[]);
    void run();

private:
    CentralBank* central_bank;
    CSVHandler* csv_handler;
    std::string read_input_args(int argc, char* argv[], std::string arg_sign);
};

std::string convert_int_to_string(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

std::string convert_double_to_string(double num, int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(PRECISION) << num;
    return ss.str();
}

Bank::Bank(uint id, ushort short_term_profit_margin, double short_term_min_investment) {
    this->id = id;
    this->short_term_profit_margin = short_term_profit_margin;
    this->short_term_min_investment = short_term_min_investment;
    this->available_deposit_id = INITIAL_AVAILABLE_ID;
}

uint Bank::get_id() {
    return this->id;
}

double Bank::calc_user_money(uint user_id) {
    double money = 0;
    for (auto deposit : this->deposits)
        if (deposit->get_user_id() == user_id)
            money += deposit->get_cur_investment();
    return money;
}

RequestResult Bank::create_deposit(uint user_id, double init_investment, DepositType type, uint years, uint short_term_link_id) {
    if (type == SHORT_TERM && init_investment < short_term_min_investment)
        return NOT_ENOUGH_MONEY;
    else if (type == LONG_TERM && init_investment < short_term_min_investment * years)
        return NOT_ENOUGH_MONEY;
    Deposit* short_term_link_deposit = find_deposit_by_id(short_term_link_id);
    if (type == LONG_TERM && (short_term_link_deposit == nullptr || short_term_link_deposit->get_user_id() != user_id))
        return INVALID_SHORT_TERM_DEPOSIT;
    uint deposit_id = DEFAULT_ID;
    if (type == SHORT_TERM) {
        deposit_id = this->available_deposit_id++;
    }
    Deposit* deposit = new Deposit(deposit_id, user_id, init_investment, type, years, short_term_link_deposit);
    this->deposits.push_back(deposit);
    RequestResult response = (deposit_id == DEFAULT_ID) ? DEPOSIT_CREATED : convert_int_to_string(deposit_id);
    return response;
}

RequestResult Bank::get_inventory_report(uint user_id, uint short_term_id) {
    Deposit* short_term_deposit = find_deposit_by_id(short_term_id);
    if (short_term_deposit == nullptr || short_term_deposit->get_user_id() != user_id)
        return INVALID_SHORT_TERM_DEPOSIT;
    double money = short_term_deposit->get_cur_investment();
    return convert_double_to_string(money, PRECISION);
}

Deposit* Bank::find_deposit_by_id(uint id) {
    for (auto deposit : this->deposits) {
        if (deposit->get_id() == id)
            return deposit;
    }
    return nullptr;
}

void Bank::pass_month() {
    for (auto deposit : deposits) {
        deposit->apply_interest(this->short_term_profit_margin);
    }
}

User::User(uint id, double wallet) {
    this->id = id;
    this->wallet = wallet;
}

uint User::get_id() {
    return this->id;
}

double User::get_wallet() {
    return this->wallet;
}

void User::decrease_wallet(double money) {
    this->wallet -= money;
}

Deposit::Deposit(uint id, uint user_id, double init_investment, DepositType type, uint years, Deposit* short_term_link) {
    this->id = id;
    this->user_id = user_id;
    this->init_investment = init_investment;
    this->cur_investment = init_investment;
    this->type = type;
    this->years = years;
    this->short_term_link = short_term_link;
}

uint Deposit::get_user_id() {
    return this->user_id;
}

uint Deposit::get_id() {
    return this->id;
}

DepositType Deposit::get_type() {
    return this->type;
}

void Deposit::add_money(double money) {
    this->cur_investment += money;
}

void Deposit::apply_interest(ushort short_term_profit_margin) {
    if (this->type == LONG_TERM)
        return apply_long_term_interest(short_term_profit_margin);
    else if (this->type == SHORT_TERM)
        return apply_short_term_interest(short_term_profit_margin);
    else if (this->type == GHARZOL)
        return apply_gharzol_interest();
}

void Deposit::apply_long_term_interest(ushort short_term_profit_margin) {
    double interest = (short_term_profit_margin * this->years) / 100.0 * this->init_investment;
    this->short_term_link->add_money(interest);
}

void Deposit::apply_short_term_interest(ushort short_term_profit_margin) {
    double interest = short_term_profit_margin / 100.0 * this->init_investment;
    this->cur_investment += interest;
}

void Deposit::apply_gharzol_interest() {
}

double Deposit::get_cur_investment() {
    return this->cur_investment;
}

CentralBank::CentralBank(std::vector<Bank*> banks, std::vector<User*> users) {
    this->banks = banks;
    this->users = users;
}

RequestResult CentralBank::handle_create_deposit(uint user_id, uint bank_id, double init_investment, DepositType type, uint years, uint short_term_link_id) {
    User* user = find_user_by_id(user_id);
    if (user->get_wallet() < init_investment)
        return NOT_ENOUGH_MONEY;
    Bank* bank = find_bank_by_id(bank_id);
    RequestResult result = bank->create_deposit(user_id, init_investment, type, years, short_term_link_id);
    if (result != NOT_ENOUGH_MONEY && result != INVALID_SHORT_TERM_DEPOSIT)
        user->decrease_wallet(init_investment);
    return result;
}

RequestResult CentralBank::handle_calc_money_in_bank(uint user_id, uint bank_id) {
    Bank* bank = find_bank_by_id(bank_id);
    double money = bank->calc_user_money(user_id);
    return convert_double_to_string(money, PRECISION);
}

RequestResult CentralBank::handle_calc_all_money(uint user_id) {
    double money = 0;
    for (auto bank : this->banks)
        money += bank->calc_user_money(user_id);
    return convert_double_to_string(money, PRECISION);
}

RequestResult CentralBank::handle_pass_time(uint num_of_months) {
    for (int i = 0; i < num_of_months; i++)
        pass_month();
    return PASS_TIME_SUCCESS;
}

RequestResult CentralBank::handle_inventory_report(uint user_id, uint bank_id, uint short_term_id) {
    Bank* bank = find_bank_by_id(bank_id);
    return bank->get_inventory_report(user_id, short_term_id);
}

void CentralBank::pass_month() {
    for (auto bank : banks)
        bank->pass_month();
}

Bank* CentralBank::find_bank_by_id(uint id) {
    for (auto bank : this->banks) {
        if (bank->get_id() == id)
            return bank;
    }
    return nullptr;
}

User* CentralBank::find_user_by_id(uint id) {
    for (auto user : this->users) {
        if (user->get_id() == id)
            return user;
    }
    return nullptr;
}

CSVHandler::CSVHandler(std::string users_file_path, std::string banks_file_path) {
    this->users_file_path = users_file_path;
    this->banks_file_path = banks_file_path;
}

std::vector<std::string> CSVHandler::parse_line(std::string line) {
    std::vector<std::string> result;
    std::string separating_word;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == CSV_WORD_PARSER) {
            result.push_back(separating_word);
            separating_word = "";
        }
        else {
            separating_word += line[i];
        }
    }
    if (separating_word.size())
        result.push_back(separating_word);
    return result;
}

Bank* CSVHandler::signup_bank(std::vector<std::string> words) {
    if (words.size() != BANK_SIGNUP_PARAMS) {
        std::cout << NOT_SUITABLE_PARAMS;
        return nullptr;
    }
    uint id = (uint)std::stoi(words[0]);
    ushort short_term_profit_margin = (ushort)std::stoi(words[1]);
    double short_term_min_investment = (double)std::stod(words[2]);
    return new Bank(id, short_term_profit_margin, short_term_min_investment);
}

std::vector<Bank*> CSVHandler::parse_and_assign_banks(std::ifstream& file) {
    std::vector<Bank*> banks;
    std::string trash;
    std::getline(file, trash);
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> words = parse_line(line);
        banks.push_back(signup_bank(words));
    }
    return banks;
}

User* CSVHandler::signup_user(std::vector<std::string> words) {
    if (words.size() != USER_SIGNUP_PARAMS) {
        std::cout << NOT_SUITABLE_PARAMS;
        return nullptr;
    }
    uint id = (uint)std::stoi(words[0]);
    double wallet = (double)std::stod(words[1]);
    return new User(id, wallet);
}

std::vector<User*> CSVHandler::parse_and_assign_users(std::ifstream& file) {
    std::vector<User*> users;
    std::string trash;
    std::getline(file, trash);
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> words = parse_line(line);
        users.push_back(signup_user(words));
    }
    return users;
}

std::vector<Bank*> CSVHandler::read_banks_file() {
    std::vector<Bank*> banks;
    std::ifstream file(this->banks_file_path);
    if (!file.is_open()) {
        std::cout << OPEN_FILE_FAILED;
        return banks;
    }
    banks = parse_and_assign_banks(file);
    return banks;
}

std::vector<User*> CSVHandler::read_users_file() {
    std::vector<User*> users;
    std::ifstream file(this->users_file_path);
    if (!file.is_open()) {
        std::cout << OPEN_FILE_FAILED;
        return users;
    }
    users = parse_and_assign_users(file);
    return users;
}

IOHandler::IOHandler(int argc, char* argv[]) {
    const std::string bank_path = read_input_args(argc, argv, BANK_ARG_SIGN);
    const std::string user_path = read_input_args(argc, argv, USER_ARG_SIGN);
    if (bank_path == NOT_FOUND_ARG || user_path == NOT_FOUND_ARG) {
        std::cout << NOT_FOUND_ARG << std::endl;
        exit(EXIT_FAILURE);
    }
    this->csv_handler = new CSVHandler(user_path, bank_path);
    std::vector<Bank*> banks = csv_handler->read_banks_file();
    std::vector<User*> users = csv_handler->read_users_file();
    this->central_bank = new CentralBank(banks, users);
}

void IOHandler::run() {
    std::string cmd;
    while (std::cin >> cmd) {
        RequestResult response;
        if (cmd == CREATE_SHORT_TERM_CMD) {
            uint user_id, bank_id;
            double init_investment;
            std::cin >> user_id >> bank_id >> init_investment;
            response = central_bank->handle_create_deposit(user_id, bank_id, init_investment, SHORT_TERM);
        }
        else if (cmd == CREATE_LONG_TERM_CMD) {
            uint user_id, bank_id, years, short_term_id;
            double init_investment;
            std::cin >> user_id >> bank_id >> short_term_id >> years >> init_investment;
            response = central_bank->handle_create_deposit(user_id, bank_id, init_investment, LONG_TERM, years, short_term_id);
        }
        else if (cmd == CREATE_GHARZOL_CMD) {
            uint user_id, bank_id;
            double init_investment;
            std::cin >> user_id >> bank_id >> init_investment;
            response = central_bank->handle_create_deposit(user_id, bank_id, init_investment, GHARZOL);
        }
        else if (cmd == INVENTORY_REPORT_CMD) {
            uint user_id, bank_id, short_term_id;
            std::cin >> user_id >> bank_id >> short_term_id;
            response = central_bank->handle_inventory_report(user_id, bank_id, short_term_id);
        }
        else if (cmd == CALC_MONEY_IN_BANK_CMD) {
            uint user_id, bank_id;
            std::cin >> user_id >> bank_id;
            response = central_bank->handle_calc_money_in_bank(user_id, bank_id);
        }
        else if (cmd == CALC_ALL_MONEY_CMD) {
            uint user_id;
            std::cin >> user_id;
            response = central_bank->handle_calc_all_money(user_id);
        }
        else if (cmd == PASS_TIME_CMD) {
            uint num_of_months;
            std::cin >> num_of_months;
            response = central_bank->handle_pass_time(num_of_months);
        }
        else {
            response = INVALID_CMD;
        }
        std::cout << response << std::endl;
    }
}

std::string IOHandler::read_input_args(int argc, char* argv[], std::string arg_sign) {
    bool is_found = false;
    for (int i = 0; i < argc; i++) {
        if (is_found)
            return argv[i];
        if (argv[i] == arg_sign)
            is_found = true;
    }
    return NOT_FOUND_ARG;
}

int main(int argc, char* argv[]) {
    IOHandler io_handler(argc, argv);
    io_handler.run();
    return 0;
}